// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#ifndef __daccess_h__
#define __daccess_h__

#include "switches.h"
#include "safemath.h"
#include "corerror.h"

#ifndef __in
#include <specstrings.h>
#endif

#define DACCESS_TABLE_RESOURCE "COREXTERNALDATAACCESSRESOURCE"

#include "clr_std/type_traits"
#include "crosscomp.h"

// Information stored in the DAC table of interest to the DAC implementation
// Note that this information is shared between all instantiations of ClrDataAccess, so initialize
// it just once in code:ClrDataAccess.GetDacGlobals (rather than use fields in ClrDataAccess);
struct DacTableInfo
{
    // On Windows, the first DWORD is the 32-bit timestamp read out of the runtime dll's debug directory.
    // The remaining 3 DWORDS must all be 0.
    // On Mac, this is the 16-byte UUID of the runtime dll.
    // It is used to validate that mscorwks is the same version as mscordacwks
    DWORD dwID0;
    DWORD dwID1;
    DWORD dwID2;
    DWORD dwID3;
};

// The header of the DAC table.  This includes the number of globals, the number of vptrs, and
// the DacTableInfo structure.  We need the DacTableInfo and DacTableHeader structs outside
// of a DACCESS_COMPILE since soshost walks the Dac table headers to find the UUID of CoreCLR
// in the target process.
struct DacTableHeader
{
    ULONG numGlobals;
    ULONG numVptrs;
    DacTableInfo info;
};

#ifdef DACCESS_COMPILE

//
// This version of things wraps pointer access in
// templates which understand how to retrieve data
// through an access layer.  In this case no assumptions
// can be made that the current compilation processor or
// pointer types match the target's processor or pointer types.
//

// Define TADDR as a non-pointer value so use of it as a pointer
// will not work properly.  Define it as unsigned so
// pointer comparisons aren't affected by sign.
// This requires special casting to ULONG64 to sign-extend if necessary.
typedef ULONG_PTR TADDR;

// TSIZE_T used for counts or ranges that need to span the size of a 
// target pointer.  For cross-plat, this may be different than SIZE_T
// which reflects the host pointer size.
typedef SIZE_T TSIZE_T;

extern DacTableInfo g_dacTableInfo;

//
// The following table contains all the global information that data access needs to begin 
// operation.  All of the values stored here are RVAs.  DacGlobalBase() returns the current 
// base address to combine with to get a full target address.
//

typedef struct _DacGlobals
{
// These will define all of the dac related mscorwks static and global variables    
#define DEFINE_DACVAR(id_type, size, id)                 id_type id;
#define DEFINE_DACVAR_NO_DUMP(id_type, size, id)        id_type id;
#include "dacvars.h"

    // Global functions.
    ULONG fn__ThreadpoolMgr__AsyncTimerCallbackCompletion;
    ULONG fn__DACNotifyCompilationFinished;
    ULONG fn__ThePreStub;
    ULONG fn__ThePreStubPatchLabel;
    ULONG fn__PrecodeFixupThunk;
    ULONG fn__StubDispatchFixupStub;
    ULONG fn__StubDispatchFixupPatchLabel;;
#ifdef FEATURE_COMINTEROP
    ULONG fn__Unknown_AddRef;
    ULONG fn__Unknown_AddRefSpecial;
    ULONG fn__Unknown_AddRefInner;
#endif

    // Vtable pointer values for all classes that must
    // be instanted using vtable pointers as the identity.
#define VPTR_CLASS(name) ULONG name##__vtAddr;
#define VPTR_MULTI_CLASS(name, keyBase) ULONG name##__##keyBase##__mvtAddr;
#include <vptr_list.h>
#undef VPTR_CLASS
#undef VPTR_MULTI_CLASS
} DacGlobals;

extern DacGlobals g_dacGlobals;

#ifdef __cplusplus
extern "C" {
#endif

// These two functions are largely just for marking code
// that is not fully converted.  DacWarning prints a debug
// message, while DacNotImpl throws a not-implemented exception.
void __cdecl DacWarning(__in __in_z char* format, ...);
void DacNotImpl(void);

void    DacError(HRESULT err);
void    DECLSPEC_NORETURN DacError_NoRet(HRESULT err);
TADDR   DacGlobalBase(void);
HRESULT DacReadAll(TADDR addr, PVOID buffer, ULONG32 size, bool throwEx);
HRESULT DacWriteAll(TADDR addr, PVOID buffer, ULONG32 size, bool throwEx);
HRESULT DacAllocVirtual(TADDR addr, ULONG32 size,
                        ULONG32 typeFlags, ULONG32 protectFlags,
                        bool throwEx, TADDR* mem);
HRESULT DacFreeVirtual(TADDR mem, ULONG32 size, ULONG32 typeFlags,
                       bool throwEx);
PVOID   DacInstantiateTypeByAddress(TADDR addr, ULONG32 size, bool throwEx);
PVOID   DacInstantiateTypeByAddressNoReport(TADDR addr, ULONG32 size, bool throwEx);
PVOID   DacInstantiateClassByVTable(TADDR addr, ULONG32 minSize, bool throwEx);

// Copy a null-terminated ascii or unicode string from the target to the host.
// Note that most of the work here is to find the null terminator.  If you know the exact length,
// then you can also just call DacInstantiateTypebyAddress.
PSTR    DacInstantiateStringA(TADDR addr, ULONG32 maxChars, bool throwEx);
PWSTR   DacInstantiateStringW(TADDR addr, ULONG32 maxChars, bool throwEx);

TADDR   DacGetTargetAddrForHostAddr(LPCVOID ptr, bool throwEx);
TADDR   DacGetTargetAddrForHostInteriorAddr(LPCVOID ptr, bool throwEx);
TADDR   DacGetTargetVtForHostVt(LPCVOID vtHost, bool throwEx);
PWSTR   DacGetVtNameW(TADDR targetVtable);

// Report a region of memory to the debugger
bool    DacEnumMemoryRegion(TADDR addr, TSIZE_T size, bool fExpectSuccess = true);

// Report a region of memory to the debugger
bool DacUpdateMemoryRegion(TADDR addr, TSIZE_T bufferSize, BYTE* buffer);

HRESULT DacWriteHostInstance(PVOID host, bool throwEx);

// This is meant to mimic the RethrowTerminalExceptions/
// SwallowAllExceptions/RethrowTransientExceptions macros to allow minidump
// gathering cancelation for details see 
// code:ClrDataAccess.EnumMemoryRegionsWrapper

// This is usable in EX_TRY exactly how RethrowTerminalExceptions et cetera
#define RethrowCancelExceptions                                         \
    if (GET_EXCEPTION()->GetHR() == COR_E_OPERATIONCANCELED)            \
    {                                                                   \
        EX_RETHROW;                                                     \
    }             

// Occasionally it's necessary to allocate some host memory for
// instance data that's created on the fly and so doesn't directly
// correspond to target memory.  These are held and freed on flush
// like other instances but can't be looked up by address.
PVOID DacAllocHostOnlyInstance(ULONG32 size, bool throwEx);

// Determines whether ASSERTs should be raised when inconsistencies in the target are detected
bool DacTargetConsistencyAssertsEnabled();

// Host instances can be marked as they are enumerated in
// order to break cycles.  This function returns true if
// the instance is already marked, otherwise it marks the
// instance and returns false.
bool DacHostPtrHasEnumMark(LPCVOID host);

// Determines if EnumMemoryRegions has been called on a method descriptor.
// This helps perf for minidumps of apps with large managed stacks.
bool DacHasMethodDescBeenEnumerated(LPCVOID pMD);

// Sets a flag indicating that EnumMemoryRegions on a method desciptor 
// has been successfully called. The function returns true if
// this flag had been previously set.
bool DacSetMethodDescEnumerated(LPCVOID pMD);

// Determines if a method descriptor is valid
BOOL DacValidateMD(LPCVOID pMD);

// Enumerate the instructions around a call site to help debugger stack walking heuristics
void DacEnumCodeForStackwalk(TADDR taCallEnd);

// Given the address and the size of a memory range which is stored in the buffer, replace all the patches 
// in the buffer with the real opcodes.  This is especially important on X64 where the unwinder needs to 
// disassemble the native instructions.
class MemoryRange;
HRESULT DacReplacePatchesInHostMemory(MemoryRange range, PVOID pBuffer);

//
// Convenience macros for EnumMemoryRegions implementations.
//
    
// Enumerate the given host instance and return
// true if the instance hasn't already been enumerated.
#define DacEnumHostDPtrMem(host) \
    (!DacHostPtrHasEnumMark(host) ? \
     (DacEnumMemoryRegion(PTR_HOST_TO_TADDR(host), sizeof(*host)), \
      true) : false)
#define DacEnumHostSPtrMem(host, type) \
    (!DacHostPtrHasEnumMark(host) ? \
     (DacEnumMemoryRegion(PTR_HOST_TO_TADDR(host), \
                          type::DacSize(PTR_HOST_TO_TADDR(host))), \
      true) : false)
#define DacEnumHostVPtrMem(host) \
    (!DacHostPtrHasEnumMark(host) ? \
     (DacEnumMemoryRegion(PTR_HOST_TO_TADDR(host), (host)->VPtrSize()), \
      true) : false)

// Check enumeration of 'this' and return if this has already been
// enumerated.  Making this the first line of an object's EnumMemoryRegions
// method will prevent cycles.
#define DAC_CHECK_ENUM_THIS() \
    if (DacHostPtrHasEnumMark(this)) return
#define DAC_ENUM_DTHIS() \
    if (!DacEnumHostDPtrMem(this)) return
#define DAC_ENUM_STHIS(type) \
    if (!DacEnumHostSPtrMem(this, type)) return
#define DAC_ENUM_VTHIS() \
    if (!DacEnumHostVPtrMem(this)) return
    
#ifdef __cplusplus
}
class ReflectionModule;
interface IMDInternalImport* DacGetMDImport(const class PEFile* peFile,
                                            bool throwEx);
interface IMDInternalImport* DacGetMDImport(const ReflectionModule* reflectionModule, 
                                            bool throwEx);

int DacGetIlMethodSize(TADDR methAddr);
struct COR_ILMETHOD* DacGetIlMethod(TADDR methAddr);
#if defined(WIN64EXCEPTIONS)
struct _UNWIND_INFO * DacGetUnwindInfo(TADDR taUnwindInfo);

// virtually unwind a CONTEXT out-of-process
struct _KNONVOLATILE_CONTEXT_POINTERS;
BOOL DacUnwindStackFrame(T_CONTEXT * pContext, T_KNONVOLATILE_CONTEXT_POINTERS* pContextPointers);
#endif // _WIN64

#ifdef FEATURE_MINIMETADATA_IN_TRIAGEDUMPS
class SString;
void DacMdCacheAddEEName(TADDR taEE, const SString& ssEEName);
bool DacMdCacheGetEEName(TADDR taEE, SString & ssEEName);
#endif // FEATURE_MINIMETADATA_IN_TRIAGEDUMPS

// 
// Computes (taBase + (dwIndex * dwElementSize()), with overflow checks.
// 
// Arguments:
//     taBase          the base TADDR value
//     dwIndex         the index of the offset
//     dwElementSize   the size of each element (to multiply the offset by)
//     
// Return value:
//     The resulting TADDR, or throws CORDB_E_TARGET_INCONSISTENT on overlow.
//     
// Notes:
//     The idea here is that overflows during address arithmetic suggest that we're operating on corrupt
//     pointers.  It helps to improve reliability to detect the cases we can (like overflow) and fail.  Note
//     that this is just a heuristic, not a security measure.  We can't trust target data regardless -
//     failing on overflow is just one easy case of corruption to detect.  There is no need to use checked
//     arithmetic everywhere in the DAC infrastructure, this is intended just for the places most likely to
//     help catch bugs (eg. __DPtr::operator[]).
// 
inline TADDR DacTAddrOffset( TADDR taBase, TSIZE_T dwIndex, TSIZE_T dwElementSize )
{
    ClrSafeInt<TADDR> t(taBase);
    t += ClrSafeInt<TSIZE_T>(dwIndex) * ClrSafeInt<TSIZE_T>(dwElementSize);
    if( t.IsOverflow() )
    {
        // Pointer arithmetic overflow - probably due to corrupt target data
        DacError(CORDBG_E_TARGET_INCONSISTENT);
    }
    return t.Value();
}


// Base pointer wrapper which provides common behavior.
class __TPtrBase
{
public:
    __TPtrBase(void)
    {
        // Make uninitialized pointers obvious.
        m_addr = (TADDR)-1;
    }
    __TPtrBase(TADDR addr)
    {
        m_addr = addr;
    }

    bool operator!() const
    {
        return m_addr == 0;
    }
    // We'd like to have an implicit conversion to bool here since the C++
    // standard says all pointer types are implicitly converted to bool. 
    // Unfortunately, that would cause ambiguous overload errors for uses
    // of operator== and operator!=.  Instead callers will have to compare
    // directly against NULL. 

    bool operator==(TADDR addr) const
    {
        return m_addr == addr;
    }
    bool operator!=(TADDR addr) const
    {
        return m_addr != addr;
    }
    bool operator<(TADDR addr) const
    {
        return m_addr < addr;
    }
    bool operator>(TADDR addr) const
    {
        return m_addr > addr;
    }
    bool operator<=(TADDR addr) const
    {
        return m_addr <= addr;
    }
    bool operator>=(TADDR addr) const
    {
        return m_addr >= addr;
    }

    TADDR GetAddr(void) const
    {
        return m_addr;
    }
    TADDR SetAddr(TADDR addr)
    {
        m_addr = addr;
        return addr;
    }

protected:
    TADDR m_addr;
};

// Pointer wrapper base class for various forms of normal data.
// This has the common functionality between __DPtr and __ArrayDPtr.
// The DPtrType type parameter is the actual derived type in use.  This is necessary so that 
// inhereted functions preserve exact return types. 
template<typename type, typename DPtrType>
class __DPtrBase : public __TPtrBase
{
public:
    typedef type _Type;
    typedef type* _Ptr;
    
protected:
    // Constructors 
    // All protected - this type should not be used directly - use one of the derived types instead.
    __DPtrBase< type, DPtrType >(void) : __TPtrBase() {}
    __DPtrBase< type, DPtrType >(TADDR addr) : __TPtrBase(addr) {}
    
    explicit __DPtrBase< type, DPtrType >(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }
    explicit __DPtrBase< type, DPtrType >(type const * host)
    {
        m_addr = DacGetTargetAddrForHostAddr(host, true);
    }

public:
    DPtrType& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return DPtrType(m_addr);
    }
    DPtrType& operator=(TADDR addr)
    {
        m_addr = addr;
        return DPtrType(m_addr);
    }

    type& operator*(void) const
    {
        return *(type*)DacInstantiateTypeByAddress(m_addr, sizeof(type), true);
    }

    bool operator==(const DPtrType& ptr) const
    {
        return m_addr == ptr.GetAddr();
    }
    bool operator==(TADDR addr) const
    {
        return m_addr == addr;
    }
    bool operator!=(const DPtrType& ptr) const
    {
        return !operator==(ptr);
    }
    bool operator!=(TADDR addr) const
    {
        return m_addr != addr;
    }
    bool operator<(const DPtrType& ptr) const
    {
        return m_addr < ptr.GetAddr();
    }
    bool operator>(const DPtrType& ptr) const
    {
        return m_addr > ptr.GetAddr();
    }
    bool operator<=(const DPtrType& ptr) const
    {
        return m_addr <= ptr.GetAddr();
    }
    bool operator>=(const DPtrType& ptr) const
    {
        return m_addr >= ptr.GetAddr();
    }

    // Array index operator
    // we want an operator[] for all possible numeric types (rather than rely on
    // implicit numeric conversions on the argument) to prevent ambiguity with 
    // DPtr's implicit conversion to type* and the built-in operator[].
    // @dbgtodo : we could also use this technique to simplify other operators below.
    template<typename indexType>
    type& operator[](indexType index)
    {
        // Compute the address of the element.
        TADDR elementAddr;
        if( index >= 0 )
        {
            elementAddr = DacTAddrOffset(m_addr, index, sizeof(type));
        }
        else
        {
            // Don't bother trying to do overflow checking for negative indexes - they are rare compared to
            // positive ones.  ClrSafeInt doesn't support signed datatypes yet (although we should be able to add it
            // pretty easily).
            elementAddr = m_addr + index * sizeof(type);
        }

        // Marshal over a single instance and return a reference to it.
        return *(type*) DacInstantiateTypeByAddress(elementAddr, sizeof(type), true);
    }

    template<typename indexType>
    type const & operator[](indexType index) const
    {
        return (*const_cast<__DPtrBase*>(this))[index];
    }

    //-------------------------------------------------------------------------
    // operator+
    
    DPtrType operator+(unsigned short val)
    {
        return DPtrType(DacTAddrOffset(m_addr, val, sizeof(type)));
    }
    DPtrType operator+(short val)
    {
        return DPtrType(m_addr + val * sizeof(type));
    }
    // size_t is unsigned int on Win32, so we need
    // to ifdef here to make sure the unsigned int
    // and size_t overloads don't collide.  size_t
    // is marked __w64 so a simple unsigned int
    // will not work on Win32, it has to be size_t.
    DPtrType operator+(size_t val)
    {
        return DPtrType(DacTAddrOffset(m_addr, val, sizeof(type)));
    }
#if defined (_WIN64)
    DPtrType operator+(unsigned int val)
    {
        return DPtrType(DacTAddrOffset(m_addr, val, sizeof(type)));
    }
#endif
    DPtrType operator+(int val)
    {
        return DPtrType(m_addr + val * sizeof(type));
    }
    // Because of the size difference between long and int on non MS compilers,
    // we only need to define these operators on Windows. These provide compatible
    // overloads for DWORD addition operations.
#ifdef _MSC_VER
    DPtrType operator+(unsigned long val)
    {
        return DPtrType(DacTAddrOffset(m_addr, val, sizeof(type)));
    }
    DPtrType operator+(long val)
    {
        return DPtrType(m_addr + val * sizeof(type));
    }
#endif

    //-------------------------------------------------------------------------
    // operator-
    
    DPtrType operator-(unsigned short val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
    DPtrType operator-(short val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
    // size_t is unsigned int on Win32, so we need
    // to ifdef here to make sure the unsigned int
    // and size_t overloads don't collide.  size_t
    // is marked __w64 so a simple unsigned int
    // will not work on Win32, it has to be size_t.
    DPtrType operator-(size_t val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
#ifdef _WIN64
    DPtrType operator-(unsigned int val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
#endif
    DPtrType operator-(int val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
    // Because of the size difference between long and int on non MS compilers,
    // we only need to define these operators on Windows. These provide compatible
    // overloads for DWORD addition operations.
#ifdef _MSC_VER // for now, everything else is 32 bit
    DPtrType operator-(unsigned long val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
    DPtrType operator-(long val)
    {
        return DPtrType(m_addr - val * sizeof(type));
    }
#endif
    size_t operator-(const DPtrType& val)
    {
        return (m_addr - val.m_addr) / sizeof(type);
    }

    //-------------------------------------------------------------------------
    
    DPtrType& operator+=(size_t val)
    {
        m_addr += val * sizeof(type);
        return static_cast<DPtrType&>(*this);
    }
    DPtrType& operator-=(size_t val)
    {
        m_addr -= val * sizeof(type);
        return static_cast<DPtrType&>(*this);
    }
    
    DPtrType& operator++()
    {
        m_addr += sizeof(type);
        return static_cast<DPtrType&>(*this);
    }
    DPtrType& operator--()
    {
        m_addr -= sizeof(type);
        return static_cast<DPtrType&>(*this);
    }
    DPtrType operator++(int postfix)
    {
        DPtrType orig = DPtrType(*this);
        m_addr += sizeof(type);
        return orig;
    }
    DPtrType operator--(int postfix)
    {
        DPtrType orig = DPtrType(*this);
        m_addr -= sizeof(type);
        return orig;
    }

    bool IsValid(void) const
    {
        return m_addr &&
            DacInstantiateTypeByAddress(m_addr, sizeof(type),
                                        false) != NULL;
    }
    void EnumMem(void) const
    {
        DacEnumMemoryRegion(m_addr, sizeof(type));
    }
};

// forward declaration
template<typename acc_type, typename store_type>
class __GlobalPtr;

// Pointer wrapper for objects which are just plain data
// and need no special handling.
template<typename type>
class __DPtr : public __DPtrBase<type,__DPtr<type> >
{
public:
    // constructors - all chain to __DPtrBase constructors
    __DPtr< type >(void) : __DPtrBase<type,__DPtr<type> >() {}
    __DPtr< type >(TADDR addr) : __DPtrBase<type,__DPtr<type> >(addr) {}

    // construct const from non-const
    typedef typename std::remove_const<type>::type mutable_type;
    __DPtr< type >(__DPtr<mutable_type> const & rhs) : __DPtrBase<type,__DPtr<type> >(rhs.GetAddr()) {}

    // construct from GlobalPtr
    explicit __DPtr< type >(__GlobalPtr< type*, __DPtr< type > > globalPtr) : 
        __DPtrBase<type,__DPtr<type> >(globalPtr.GetAddr()) {} 

    explicit __DPtr< type >(__TPtrBase addr) : __DPtrBase<type,__DPtr<type> >(addr) {}
    explicit __DPtr< type >(type const * host) : __DPtrBase<type,__DPtr<type> >(host) {}

    operator type*() const
    {
        return (type*)DacInstantiateTypeByAddress(this->m_addr, sizeof(type), true);
    }
    type* operator->() const
    {
        return (type*)DacInstantiateTypeByAddress(this->m_addr, sizeof(type), true);
    }
};

#define DPTR(type) __DPtr< type >

// A restricted form of DPtr that doesn't have any conversions to pointer types.
// This is useful for pointer types that almost always represent arrays, as opposed
// to pointers to single instances (eg. PTR_BYTE).  In these cases, allowing implicit
// conversions to (for eg.) BYTE* would usually result in incorrect usage (eg. pointer 
// arithmetic and array indexing), since only a single instance has been marshalled to the host.
// If you really must marshal a single instance (eg. converting T* to PTR_T is too painful for now),
// then use code:DacUnsafeMarshalSingleElement so we can identify such unsafe code.
template<typename type>
class __ArrayDPtr : public __DPtrBase<type,__ArrayDPtr<type> >
{
public:
    // constructors - all chain to __DPtrBase constructors
    __ArrayDPtr< type >(void) : __DPtrBase<type,__ArrayDPtr<type> >() {}
    __ArrayDPtr< type >(TADDR addr) : __DPtrBase<type,__ArrayDPtr<type> >(addr) {}

    // construct const from non-const
    typedef typename std::remove_const<type>::type mutable_type;
    __ArrayDPtr< type >(__ArrayDPtr<mutable_type> const & rhs) : __DPtrBase<type,__ArrayDPtr<type> >(rhs.GetAddr()) {}  

    explicit __ArrayDPtr< type >(__TPtrBase addr) : __DPtrBase<type,__ArrayDPtr<type> >(addr) {}

    // Note that there is also no explicit constructor from host instances (type*).
    // Going this direction is less problematic, but often still represents risky coding.
};

#define ArrayDPTR(type) __ArrayDPtr< type >


// Pointer wrapper for objects which are just plain data
// but whose size is not the same as the base type size.
// This can be used for prefetching data for arrays or
// for cases where an object has a variable size.
template<typename type>
class __SPtr : public __TPtrBase
{
public:
    typedef type _Type;
    typedef type* _Ptr;
    
    __SPtr< type >(void) : __TPtrBase() {}
    __SPtr< type >(TADDR addr) : __TPtrBase(addr) {}
    explicit __SPtr< type >(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }
    explicit __SPtr< type >(type* host)
    {
        m_addr = DacGetTargetAddrForHostAddr(host, true);
    }

    __SPtr< type >& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return *this;
    }
    __SPtr< type >& operator=(TADDR addr)
    {
        m_addr = addr;
        return *this;
    }

    operator type*() const
    {
        if (m_addr)
        {
            return (type*)DacInstantiateTypeByAddress(m_addr,
                                                      type::DacSize(m_addr),
                                                      true);
        }
        else
        {
            return (type*)NULL;
        }
    }
    type* operator->() const
    {
        if (m_addr)
        {
            return (type*)DacInstantiateTypeByAddress(m_addr,
                                                      type::DacSize(m_addr),
                                                      true);
        }
        else
        {
            return (type*)NULL;
        }
    }
    type& operator*(void) const
    {
        if (!m_addr)
        {
            DacError(E_INVALIDARG);
        }
        
        return *(type*)DacInstantiateTypeByAddress(m_addr,
                                                   type::DacSize(m_addr),
                                                   true);
    }

    bool IsValid(void) const
    {
        return m_addr &&
            DacInstantiateTypeByAddress(m_addr, type::DacSize(m_addr),
                                        false) != NULL;
    }
    void EnumMem(void) const
    {
        if (m_addr)
        {
            DacEnumMemoryRegion(m_addr, type::DacSize(m_addr));
        }
    }
};

#define SPTR(type) __SPtr< type >

// Pointer wrapper for objects which have a single leading
// vtable, such as objects in a single-inheritance tree.
// The base class of all such trees must have use
// VPTR_BASE_VTABLE_CLASS in their declaration and all
// instantiable members of the tree must be listed in vptr_list.h.
template<class type>
class __VPtr : public __TPtrBase
{
public:
    // VPtr::_Type has to be a pointer as
    // often the type is an abstract class.
    // This type is not expected to be used anyway.
    typedef type* _Type;
    typedef type* _Ptr;
    
    __VPtr< type >(void) : __TPtrBase() {}
    __VPtr< type >(TADDR addr) : __TPtrBase(addr) {}
    explicit __VPtr< type >(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }
    explicit __VPtr< type >(type* host)
    {
        m_addr = DacGetTargetAddrForHostAddr(host, true);
    }
    
    __VPtr< type >& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return *this;
    }
    __VPtr< type >& operator=(TADDR addr)
    {
        m_addr = addr;
        return *this;
    }

    operator type*() const
    {
        return (type*)DacInstantiateClassByVTable(m_addr, sizeof(type), true);
    }
    type* operator->() const
    {
        return (type*)DacInstantiateClassByVTable(m_addr, sizeof(type), true);
    }
    
    bool operator==(const __VPtr< type >& ptr) const
    {
        return m_addr == ptr.m_addr;
    }
    bool operator==(TADDR addr) const
    {
        return m_addr == addr;
    }
    bool operator!=(const __VPtr< type >& ptr) const
    {
        return !operator==(ptr);
    }
    bool operator!=(TADDR addr) const
    {
        return m_addr != addr;
    }

    bool IsValid(void) const
    {
        return m_addr &&
            DacInstantiateClassByVTable(m_addr, sizeof(type), false) != NULL;
    }
    void EnumMem(void) const
    {
        if (IsValid())
        {
            DacEnumMemoryRegion(m_addr, (operator->())->VPtrSize());
        }
    }
};

#define VPTR(type) __VPtr< type >

// Pointer wrapper for 8-bit strings.
template<typename type, ULONG32 maxChars = 32760>
class __Str8Ptr : public __DPtr<char>
{
public:
    typedef type _Type;
    typedef type* _Ptr;
    
    __Str8Ptr< type, maxChars >(void) : __DPtr<char>() {}
    __Str8Ptr< type, maxChars >(TADDR addr) : __DPtr<char>(addr) {}
    explicit __Str8Ptr< type, maxChars >(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }
    explicit __Str8Ptr< type, maxChars >(type* host)
    {
        m_addr = DacGetTargetAddrForHostAddr(host, true);
    }

    __Str8Ptr< type, maxChars >& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return *this;
    }
    __Str8Ptr< type, maxChars >& operator=(TADDR addr)
    {
        m_addr = addr;
        return *this;
    }

    operator type*() const
    {
        return (type*)DacInstantiateStringA(m_addr, maxChars, true);
    }

    bool IsValid(void) const
    {
        return m_addr &&
            DacInstantiateStringA(m_addr, maxChars, false) != NULL;
    }
    void EnumMem(void) const
    {
        char* str = DacInstantiateStringA(m_addr, maxChars, false);
        if (str)
        {
            DacEnumMemoryRegion(m_addr, strlen(str) + 1);
        }
    }
};

#define S8PTR(type) __Str8Ptr< type >
#define S8PTRMAX(type, maxChars) __Str8Ptr< type, maxChars >

// Pointer wrapper for 16-bit strings.
template<typename type, ULONG32 maxChars = 32760>
class __Str16Ptr : public __DPtr<wchar_t>
{
public:
    typedef type _Type;
    typedef type* _Ptr;
    
    __Str16Ptr< type, maxChars >(void) : __DPtr<wchar_t>() {}
    __Str16Ptr< type, maxChars >(TADDR addr) : __DPtr<wchar_t>(addr) {}
    explicit __Str16Ptr< type, maxChars >(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }
    explicit __Str16Ptr< type, maxChars >(type* host)
    {
        m_addr = DacGetTargetAddrForHostAddr(host, true);
    }

    __Str16Ptr< type, maxChars >& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return *this;
    }
    __Str16Ptr< type, maxChars >& operator=(TADDR addr)
    {
        m_addr = addr;
        return *this;
    }

    operator type*() const
    {
        return (type*)DacInstantiateStringW(m_addr, maxChars, true);
    }

    bool IsValid(void) const
    {
        return m_addr &&
            DacInstantiateStringW(m_addr, maxChars, false) != NULL;
    }
    void EnumMem(void) const
    {
        char* str = DacInstantiateStringW(m_addr, maxChars, false);
        if (str)
        {
            DacEnumMemoryRegion(m_addr, strlen(str) + 1);
        }
    }
};

#define S16PTR(type) __Str16Ptr< type >
#define S16PTRMAX(type, maxChars) __Str16Ptr< type, maxChars >

template<typename type>
class __GlobalVal
{
public:
    __GlobalVal< type >(PULONG rvaPtr)
    {
        m_rvaPtr = rvaPtr;
    }

    operator type() const
    {
        return (type)*__DPtr< type >(DacGlobalBase() + *m_rvaPtr);
    }

    __DPtr< type > operator&() const
    {
        return __DPtr< type >(DacGlobalBase() + *m_rvaPtr);
    }

    // @dbgtodo  dac support: This updates values in the host.  This seems extremely dangerous
    // to do silently.  I'd prefer that a specific (searchable) write function
    // was used.  Try disabling this and see what fails...
    __GlobalVal<type> & operator=(const type & val)
    {
        type* ptr = __DPtr< type >(DacGlobalBase() + *m_rvaPtr);
        // Update the host copy;
        *ptr = val;
        // Write back to the target.
        DacWriteHostInstance(ptr, true);
        return *this;
    }
    
    bool IsValid(void) const
    {
        return __DPtr< type >(DacGlobalBase() + *m_rvaPtr).IsValid();
    }
    void EnumMem(void) const
    {
        TADDR p = DacGlobalBase() + *m_rvaPtr;
        __DPtr< type >(p).EnumMem();
    }

private:
    PULONG m_rvaPtr;
};

template<typename type, size_t size>
class __GlobalArray
{
public:
    __GlobalArray< type, size >(PULONG rvaPtr)
    {
        m_rvaPtr = rvaPtr;
    }

    __DPtr< type > operator&() const
    {
        return __DPtr< type >(DacGlobalBase() + *m_rvaPtr);
    }
    
    type& operator[](unsigned int index) const
    {
        return __DPtr< type >(DacGlobalBase() + *m_rvaPtr)[index];
    }
    
    bool IsValid(void) const
    {
        // Only validates the base pointer, not the full array range.
        return __DPtr< type >(DacGlobalBase() + *m_rvaPtr).IsValid();
    }
    void EnumMem(void) const
    {
        DacEnumMemoryRegion(DacGlobalBase() + *m_rvaPtr, sizeof(type) * size);
    }

private:
    PULONG m_rvaPtr;
};

template<typename acc_type, typename store_type>
class __GlobalPtr
{
public:
    __GlobalPtr< acc_type, store_type >(PULONG rvaPtr)
    {
        m_rvaPtr = rvaPtr;
    }

    __DPtr< store_type > operator&() const
    {
        return __DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
    }
      
    store_type & operator=(store_type & val)
    {
        store_type* ptr = __DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
        // Update the host copy;
        *ptr = val;
        // Write back to the target.
        DacWriteHostInstance(ptr, true);
        return val;
    }
    
    acc_type operator->() const
    {
        return (acc_type)*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
    }
    operator acc_type() const
    {
        return (acc_type)*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
    }
    operator store_type() const
    {
        return *__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
    }
    bool operator!() const
    {
        return !*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr);
    }

    typename store_type::_Type& operator[](int index)
    {
        return (*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr))[index];
    }
    
    typename store_type::_Type& operator[](unsigned int index)
    {
        return (*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr))[index];
    }

    TADDR GetAddr() const
    {
        return (*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr)).GetAddr();
    }

    TADDR GetAddrRaw () const
    {
        return DacGlobalBase() + *m_rvaPtr;
    }
    
    // This is only testing the the pointer memory is available but does not verify
    // the memory that it points to.
    //
    bool IsValidPtr(void) const
    {
        return __DPtr< store_type >(DacGlobalBase() + *m_rvaPtr).IsValid();
    }
    
    bool IsValid(void) const
    {
        return __DPtr< store_type >(DacGlobalBase() + *m_rvaPtr).IsValid() &&
            (*__DPtr< store_type >(DacGlobalBase() + *m_rvaPtr)).IsValid();
    }
    void EnumMem(void) const
    {
        __DPtr< store_type > ptr(DacGlobalBase() + *m_rvaPtr);
        ptr.EnumMem();
        if (ptr.IsValid())
        {
            (*ptr).EnumMem();
        }
    }

    PULONG m_rvaPtr;
};

template<typename acc_type, typename store_type>
inline bool operator==(const __GlobalPtr<acc_type, store_type>& gptr,
                       acc_type host)
{
    return DacGetTargetAddrForHostAddr(host, true) ==
        *__DPtr< TADDR >(DacGlobalBase() + *gptr.m_rvaPtr);
}
template<typename acc_type, typename store_type>
inline bool operator!=(const __GlobalPtr<acc_type, store_type>& gptr,
                       acc_type host)
{
    return !operator==(gptr, host);
}

template<typename acc_type, typename store_type>
inline bool operator==(acc_type host,
                       const __GlobalPtr<acc_type, store_type>& gptr)
{
    return DacGetTargetAddrForHostAddr(host, true) ==
        *__DPtr< TADDR >(DacGlobalBase() + *gptr.m_rvaPtr);
}
template<typename acc_type, typename store_type>
inline bool operator!=(acc_type host,
                       const __GlobalPtr<acc_type, store_type>& gptr)
{
    return !operator==(host, gptr);
}


// 
// __VoidPtr is a type that behaves like void* but for target pointers.
// Behavior of PTR_VOID:
// * has void* semantics. Will compile to void* in non-DAC builds (just like
//     other PTR types. Unlike TADDR, we want pointer semantics.
// * NOT assignable from host pointer types or convertible to host pointer
//     types - ensures we can't confuse host and target pointers (we'll get
//     compiler errors if we try and cast between them).
// * like void*, no pointer arithmetic or dereferencing is allowed
// * like TADDR, can be used to construct any __DPtr / __VPtr instance
// * representation is the same as a void* (for marshalling / casting)
//     
// One way in which __VoidPtr is unlike void* is that it can't be cast to
// pointer or integer types. On the one hand, this is a good thing as it forces
// us to keep target pointers separate from other data types. On the other hand
// in practice this means we have to use dac_cast<TADDR> in places where we used
// to use a (TADDR) cast. Unfortunately C++ provides us no way to allow the
// explicit cast to primitive types without also allowing implicit conversions.
// 
// This is very similar in spirit to TADDR. The primary difference is that
// PTR_VOID has pointer semantics, where TADDR has integer semantics. When
// dacizing uses of void* to TADDR, casts must be inserted everywhere back to
// pointer types. If we switch a use of TADDR to PTR_VOID, those casts in
// DACCESS_COMPILE regions no longer compile (see above). Also, TADDR supports
// pointer arithmetic, but that might not be necessary (could use PTR_BYTE
// instead etc.). Ideally we'd probably have just one type for this purpose
// (named TADDR but with the semantics of PTR_VOID), but outright conversion
// would require too much work.
// 
class __VoidPtr : public __TPtrBase
{
public:
    __VoidPtr(void) : __TPtrBase() {}
    __VoidPtr(TADDR addr) : __TPtrBase(addr) {}

    // Note, unlike __DPtr, this ctor form is not explicit.  We allow implicit
    // conversions from any pointer type (just like for void*).
    __VoidPtr(__TPtrBase addr)
    {
        m_addr = addr.GetAddr();
    }

    // Like TPtrBase, VoidPtrs can also be created impicitly from all GlobalPtrs 
    template<typename acc_type, typename store_type>
    __VoidPtr(__GlobalPtr<acc_type, store_type> globalPtr)
    {
        m_addr = globalPtr.GetAddr();
    }

    // Note, unlike __DPtr, there is no explicit conversion from host pointer
    // types.  Since void* cannot be marshalled, there is no such thing as
    // a void* DAC instance in the host.

    // Also, we don't want an implicit conversion to TADDR because then the
    // compiler will allow pointer arithmetic (which it wouldn't allow for
    // void*).  Instead, callers can use dac_cast<TADDR> if they want.

    // Note, unlike __DPtr, any pointer type can be assigned to a __VoidPtr
    // This is to mirror the assignability of any pointer type to a void*
    __VoidPtr& operator=(const __TPtrBase& ptr)
    {
        m_addr = ptr.GetAddr();
        return *this;
    }
    __VoidPtr& operator=(TADDR addr)
    {
        m_addr = addr;
        return *this;
    }

    // note, no marshalling operators (type* conversion, operator ->, operator*)
    // A void* can't be marshalled because we don't know how much to copy

    // PTR_Void can be compared to any other pointer type (because conceptually,
    // any other pointer type should be implicitly convertible to void*)
    bool operator==(const __TPtrBase& ptr) const
    {
        return m_addr == ptr.GetAddr();
    }
    bool operator==(TADDR addr) const
    {
        return m_addr == addr;
    }
    bool operator!=(const __TPtrBase& ptr) const
    {
        return !operator==(ptr);
    }
    bool operator!=(TADDR addr) const
    {
        return m_addr != addr;
    }
    bool operator<(const __TPtrBase& ptr) const
    {
        return m_addr < ptr.GetAddr();
    }
    bool operator>(const __TPtrBase& ptr) const
    {
        return m_addr > ptr.GetAddr();
    }
    bool operator<=(const __TPtrBase& ptr) const
    {
        return m_addr <= ptr.GetAddr();
    }
    bool operator>=(const __TPtrBase& ptr) const
    {
        return m_addr >= ptr.GetAddr();
    }   
};

typedef __VoidPtr PTR_VOID;
typedef DPTR(PTR_VOID) PTR_PTR_VOID;

// For now we treat pointers to const and non-const void the same in DAC
// builds. In general, DAC is read-only anyway and so there isn't a danger of
// writing to these pointers. Also, the non-dac builds will ensure
// const-correctness. However, if we wanted to support true void* / const void*
// behavior, we could probably build the follow functionality by templating
// __VoidPtr:
//  * A PTR_VOID would be implicitly convertable to PTR_CVOID
//  * An explicit coercion (ideally const_cast) would be required to convert a
//      PTR_CVOID to a PTR_VOID
//  * Similarily, an explicit coercion would be required to convert a cost PTR
//      type (eg. PTR_CBYTE) to a PTR_VOID.
typedef __VoidPtr PTR_CVOID;


// The special empty ctor declared here allows the whole
// class hierarchy to be instantiated easily by the
// external access code.  The actual class body will be
// read externally so no members should be initialized.

//
// VPTR_ANY_CLASS_METHODS - Defines the following methods for all VPTR classes
// 
// VPtrSize 
//     Returns the size of the dynamic type of the object (as opposed to sizeof
//     which is based only on the static type). 
//     
// VPtrHostVTable
//     Returns the address of the vtable for this type.
//     We create a temporary instance of this type in order to read it's vtable pointer 
//     (at offset 0).  For this temporary instance, we do not want to initialize any fields,
//     so we use the marshalling ctor.  Since we didn't initialize any fields, we also don't
//     wan't to run the dtor (marshaled data structures don't normally expect their destructor 
//     or non-DAC constructors to be called in DAC builds anyway).  So, rather than create a 
//     normal stack object, or put the object on the heap, we create the temporary object 
//     on the stack using placement-new and alloca, and don't destruct it.
//     
#define VPTR_ANY_CLASS_METHODS(name)                            \
        virtual ULONG32 VPtrSize(void) { SUPPORTS_DAC; return sizeof(name); } \
        static PVOID VPtrHostVTable() {                         \
            void * pBuf = _alloca(sizeof(name));                \
            name * dummy = new (pBuf) name((TADDR)0, (TADDR)0); \
            return *((PVOID*)dummy); }

#define VPTR_CLASS_METHODS(name)                                \
        VPTR_ANY_CLASS_METHODS(name)                            \
        static TADDR VPtrTargetVTable() {                       \
            SUPPORTS_DAC;                                       \
            return DacGlobalBase() + g_dacGlobals.name##__vtAddr; }

#define VPTR_MULTI_CLASS_METHODS(name, keyBase)                 \
        VPTR_ANY_CLASS_METHODS(name)                            \
        static TADDR VPtrTargetVTable() {                       \
            SUPPORTS_DAC;                                       \
            return DacGlobalBase() + g_dacGlobals.name##__##keyBase##__mvtAddr; }

#define VPTR_VTABLE_CLASS(name, base)                           \
public: name(TADDR addr, TADDR vtAddr) : base(addr, vtAddr) {}  \
        VPTR_CLASS_METHODS(name)

#define VPTR_MULTI_VTABLE_CLASS(name, base)                     \
public: name(TADDR addr, TADDR vtAddr) : base(addr, vtAddr) {}  \
        VPTR_MULTI_CLASS_METHODS(name, base)

// Used for base classes that can be instantiated directly.
// The fake vfn is still used to force a vtable even when
// all the normal vfns are ifdef'ed out.
#define VPTR_BASE_CONCRETE_VTABLE_CLASS(name)                   \
public: name(TADDR addr, TADDR vtAddr) {}                       \
        VPTR_CLASS_METHODS(name)

#define VPTR_BASE_CONCRETE_VTABLE_CLASS_NO_CTOR_BODY(name)      \
public: name(TADDR addr, TADDR vtAddr);                         \
        VPTR_CLASS_METHODS(name)

// The pure virtual method forces all derivations to use
// VPTR_VTABLE_CLASS to compile.
#define VPTR_BASE_VTABLE_CLASS(name)                            \
public: name(TADDR addr, TADDR vtAddr) {}                       \
        virtual ULONG32 VPtrSize(void) = 0;

#define VPTR_BASE_VTABLE_CLASS_NO_CTOR_BODY(name)               \
public: name(TADDR addr, TADDR vtAddr);                         \
        virtual ULONG32 VPtrSize(void) = 0;

#define VPTR_ABSTRACT_VTABLE_CLASS(name, base)                  \
public: name(TADDR addr, TADDR vtAddr) : base(addr, vtAddr) {}

#define VPTR_ABSTRACT_VTABLE_CLASS_NO_CTOR_BODY(name, base)     \
public: name(TADDR addr, TADDR vtAddr);

// helper macro to make the vtables unique for DAC
#define VPTR_UNIQUE(unique)

// Safe access for retrieving the target address of a PTR.
#define PTR_TO_TADDR(ptr) ((ptr).GetAddr())

#define GFN_TADDR(name) (DacGlobalBase() + g_dacGlobals.fn__ ## name)

// ROTORTODO - g++ 3 doesn't like the use of the operator& in __GlobalVal
// here. Putting GVAL_ADDR in to get things to compile while I discuss
// this matter with the g++ authors.

#define GVAL_ADDR(g) \
    ((g).operator&())

//
// References to class static and global data.
// These all need to be redirected through the global
// data table.
//

#define _SPTR_DECL(acc_type, store_type, var) \
    static __GlobalPtr< acc_type, store_type > var
#define _SPTR_IMPL(acc_type, store_type, cls, var) \
    __GlobalPtr< acc_type, store_type > cls::var(&g_dacGlobals.cls##__##var)
#define _SPTR_IMPL_INIT(acc_type, store_type, cls, var, init) \
    __GlobalPtr< acc_type, store_type > cls::var(&g_dacGlobals.cls##__##var)
#define _SPTR_IMPL_NS(acc_type, store_type, ns, cls, var) \
    __GlobalPtr< acc_type, store_type > cls::var(&g_dacGlobals.ns##__##cls##__##var)
#define _SPTR_IMPL_NS_INIT(acc_type, store_type, ns, cls, var, init) \
    __GlobalPtr< acc_type, store_type > cls::var(&g_dacGlobals.ns##__##cls##__##var)

#define _GPTR_DECL(acc_type, store_type, var) \
    extern __GlobalPtr< acc_type, store_type > var
#define _GPTR_IMPL(acc_type, store_type, var) \
    __GlobalPtr< acc_type, store_type > var(&g_dacGlobals.dac__##var)
#define _GPTR_IMPL_INIT(acc_type, store_type, var, init) \
    __GlobalPtr< acc_type, store_type > var(&g_dacGlobals.dac__##var)

#define SVAL_DECL(type, var) \
    static __GlobalVal< type > var
#define SVAL_IMPL(type, cls, var) \
    __GlobalVal< type > cls::var(&g_dacGlobals.cls##__##var)
#define SVAL_IMPL_INIT(type, cls, var, init) \
    __GlobalVal< type > cls::var(&g_dacGlobals.cls##__##var)
#define SVAL_IMPL_NS(type, ns, cls, var) \
    __GlobalVal< type > cls::var(&g_dacGlobals.ns##__##cls##__##var)
#define SVAL_IMPL_NS_INIT(type, ns, cls, var, init) \
    __GlobalVal< type > cls::var(&g_dacGlobals.ns##__##cls##__##var)

#define GVAL_DECL(type, var) \
    extern __GlobalVal< type > var
#define GVAL_IMPL(type, var) \
    __GlobalVal< type > var(&g_dacGlobals.dac__##var)
#define GVAL_IMPL_INIT(type, var, init) \
    __GlobalVal< type > var(&g_dacGlobals.dac__##var)

#define GARY_DECL(type, var, size) \
    extern __GlobalArray< type, size > var
#define GARY_IMPL(type, var, size) \
    __GlobalArray< type, size > var(&g_dacGlobals.dac__##var)

// Translation from a host pointer back to the target address
// that was used to retrieve the data for the host pointer.
#define PTR_HOST_TO_TADDR(host) DacGetTargetAddrForHostAddr(host, true)
// Translation from a host interior pointer back to the corresponding
// target address. The host address must reside within a previously
// retrieved instance.
#define PTR_HOST_INT_TO_TADDR(host) DacGetTargetAddrForHostInteriorAddr(host, true)
// Translation from a host vtable pointer to a target vtable pointer.
#define VPTR_HOST_VTABLE_TO_TADDR(host) DacGetTargetVtForHostVt(host, true)

// Construct a pointer to a member of the given type.
#define PTR_HOST_MEMBER_TADDR(type, host, memb) \
    (PTR_HOST_TO_TADDR(host) + (TADDR)offsetof(type, memb))

// Construct a pointer to a member of the given type given an interior
// host address.
#define PTR_HOST_INT_MEMBER_TADDR(type, host, memb) \
    (PTR_HOST_INT_TO_TADDR(host) + (TADDR)offsetof(type, memb))

#define PTR_TO_MEMBER_TADDR(type, ptr, memb) \
    (PTR_TO_TADDR(ptr) + (TADDR)offsetof(type, memb))

// Constructs an arbitrary data instance for a piece of
// memory in the target.
#define PTR_READ(addr, size) \
    DacInstantiateTypeByAddress(addr, size, true)

// This value is used to intiailize target pointers to NULL.  We want this to be TADDR type
// (as opposed to, say, __TPtrBase) so that it can be used in the non-explicit ctor overloads,
// eg. as an argument default value.  
// We can't always just use NULL because that's 0 which (in C++) can be any integer or pointer 
// type (causing an ambiguous overload compiler error when used in explicit ctor forms).
#define PTR_NULL ((TADDR)0)

// Provides an empty method implementation when compiled
// for DACCESS_COMPILE.  For example, use to stub out methods needed
// for vtable entries but otherwise unused.
// Note that these functions are explicitly NOT marked SUPPORTS_DAC so that we'll get a 
// DacCop warning if any calls to them are detected.
// @dbgtodo : It's probably almost always wrong to call any such function, so
// we should probably throw a better error (DacNotImpl), and ideally mark the function
// DECLSPEC_NORETURN so we don't have to deal with fabricating return values and we can
// get compiler warnings (unreachable code) anytime functions marked this way are called.
#define DAC_EMPTY() { LIMITED_METHOD_CONTRACT; }
#define DAC_EMPTY_ERR() { LIMITED_METHOD_CONTRACT; DacError(E_UNEXPECTED); }
#define DAC_EMPTY_RET(retVal) { LIMITED_METHOD_CONTRACT; DacError(E_UNEXPECTED); return retVal; }
#define DAC_UNEXPECTED() { LIMITED_METHOD_CONTRACT; DacError_NoRet(E_UNEXPECTED); }

#endif // #ifdef __cplusplus

// Implementation details for dac_cast, should never be accessed directly.
// See code:dac_cast for details and discussion.
namespace dac_imp
{
    // Helper functions to get the target address of specific types
    inline TADDR getTaddr(TADDR addr) { return addr; }
    inline TADDR getTaddr(__TPtrBase const &tptr) { return PTR_TO_TADDR(tptr); }
    inline TADDR getTaddr(void const * host) { return PTR_HOST_TO_TADDR((void *)host); }
    template<typename acc_type, typename store_type>
    inline TADDR getTaddr(__GlobalPtr<acc_type, store_type> const &gptr) { return PTR_TO_TADDR(gptr); }

    // It is an error to try dac_cast on a __GlobalVal or a __GlobalArray. Declare
    // but do not define the methods so that a compile-time error results.
    template<typename type>
    TADDR getTaddr(__GlobalVal<type> const &gval);
    template<typename type, size_t size>
    TADDR getTaddr(__GlobalArray<type, size> const &garr);

    // Helper class to instantiate DAC instances from a TADDR
    // The default implementation assumes we want to create an instance of a PTR type
    template<typename T> struct makeDacInst
    {
        static inline T fromTaddr(TADDR addr)
        {
            static_assert((std::is_base_of<__TPtrBase, T>::value), "is_base_of constraint violation");
            return T(addr);
        }
    };

    // Partial specialization for creating TADDRs
    // This is the only other way to create a DAC type instance other than PTR types (above)
    template<> struct makeDacInst<TADDR>
    {
        static inline TADDR fromTaddr(TADDR addr) { return addr; }
    };
} // namespace dac_imp


// DacCop in-line exclusion mechanism

// Warnings - official home is DacCop\Shared\Warnings.cs, but we want a way for users to indicate
// warning codes in a way that is descriptive to readers (not just code numbers).  The names here
// don't matter - DacCop just looks at the value
enum DacCopWarningCode
{
    // General Rules
    FieldAccess = 1,
    PointerArith = 2,
    PointerComparison = 3,
    InconsistentMarshalling = 4,
    CastBetweenAddressSpaces = 5,
    CastOfMarshalledType = 6,
    VirtualCallToNonVPtr = 7,
    UndacizedGlobalVariable = 8,

    // Function graph related
    CallUnknown = 701,
    CallNonDac = 702,
    CallVirtualUnknown = 704,
    CallVirtualNonDac = 705,
};

// DACCOP_IGNORE is a mechanism to suppress DacCop violations from within the source-code.
// See the DacCop wiki for guidance on how best to use this: http://mswikis/clr/dev/Pages/DacCop.aspx
//
// DACCOP_IGNORE will suppress a DacCop violation for the following (non-compound) statement.
// For example:
//      // The "dual-mode DAC problem" occurs in a few places where a class is used both
//      // in the host, and marshalled from the target ... <further details>
//      DACCOP_IGNORE(CastBetweenAddressSpaces,"SBuffer has the dual-mode DAC problem");
//      TADDR bufAddr = (TADDR)m_buffer;
//
// A call to DACCOP_IGNORE must occur as it's own statement, and can apply only to following
// single-statements (not to compound statement blocks).  Occasionally it is necessary to hoist
// violation-inducing code out to its own statement (e.g., if it occurs in the conditional of an
// if).
//
// Arguments:
//   code: a literal value from DacCopWarningCode indicating which violation should be suppressed.  
//   szReasonString: a short description of why this exclusion is necessary.  This is intended just
//        to help readers of the code understand the source of the problem, and what would be required
//        to fix it.  More details can be provided in comments if desired.
//
inline void DACCOP_IGNORE(DacCopWarningCode code, const char * szReasonString)
{
    // DacCop detects calls to this function.  No implementation is necessary.
}

#else // #ifdef DACCESS_COMPILE

//
// This version of the macros turns into normal pointers
// for unmodified in-proc compilation.

// *******************************************************
// !!!!!!!!!!!!!!!!!!!!!!!!!NOTE!!!!!!!!!!!!!!!!!!!!!!!!!!
// 
// Please search this file for the type name to find the 
// DAC versions of these definitions
// 
// !!!!!!!!!!!!!!!!!!!!!!!!!NOTE!!!!!!!!!!!!!!!!!!!!!!!!!!
// *******************************************************


// Declare TADDR as a non-pointer type so that arithmetic
// can be done on it directly, as with the DACCESS_COMPILE definition.
// This also helps expose pointer usage that may need to be changed.
typedef ULONG_PTR TADDR;
    
typedef void* PTR_VOID;
typedef LPVOID* PTR_PTR_VOID;
typedef const void* PTR_CVOID;

#define DPTR(type) type*
#define ArrayDPTR(type) type*
#define SPTR(type) type*
#define VPTR(type) type*
#define S8PTR(type) type*
#define S8PTRMAX(type, maxChars) type*
#define S16PTR(type) type*
#define S16PTRMAX(type, maxChars) type*
#define VPTR_VTABLE_CLASS(name, base)
#define VPTR_MULTI_VTABLE_CLASS(name, base)
#define VPTR_BASE_CONCRETE_VTABLE_CLASS(name)
#define VPTR_BASE_VTABLE_CLASS(name)
#define VPTR_ABSTRACT_VTABLE_CLASS(name, base)

// helper macro to make the vtables unique for DAC
#define VPTR_UNIQUE(unique) virtual int MakeVTableUniqueForDAC() {    STATIC_CONTRACT_SO_TOLERANT; return unique; }
#define VPTR_UNIQUE_BaseDomain                          (100000)
#define VPTR_UNIQUE_SystemDomain                        (VPTR_UNIQUE_BaseDomain + 1)
#define VPTR_UNIQUE_ComMethodFrame                      (VPTR_UNIQUE_SystemDomain + 1)
#define VPTR_UNIQUE_StubHelperFrame                     (VPTR_UNIQUE_ComMethodFrame + 1)
#define VPTR_UNIQUE_RedirectedThreadFrame               (VPTR_UNIQUE_StubHelperFrame + 1)
#define VPTR_UNIQUE_HijackFrame                         (VPTR_UNIQUE_RedirectedThreadFrame + 1)

#define PTR_TO_TADDR(ptr) ((TADDR)(ptr))
#define GFN_TADDR(name) ((TADDR)(name))

#define GVAL_ADDR(g) (&(g))
#define _SPTR_DECL(acc_type, store_type, var) \
    static store_type var
#define _SPTR_IMPL(acc_type, store_type, cls, var) \
    store_type cls::var
#define _SPTR_IMPL_INIT(acc_type, store_type, cls, var, init) \
    store_type cls::var = init
#define _SPTR_IMPL_NS(acc_type, store_type, ns, cls, var) \
    store_type cls::var
#define _SPTR_IMPL_NS_INIT(acc_type, store_type, ns, cls, var, init) \
    store_type cls::var = init
#define _GPTR_DECL(acc_type, store_type, var) \
    extern store_type var
#define _GPTR_IMPL(acc_type, store_type, var) \
    store_type var
#define _GPTR_IMPL_INIT(acc_type, store_type, var, init) \
    store_type var = init
#define SVAL_DECL(type, var) \
    static type var
#define SVAL_IMPL(type, cls, var) \
    type cls::var
#define SVAL_IMPL_INIT(type, cls, var, init) \
    type cls::var = init
#define SVAL_IMPL_NS(type, ns, cls, var) \
    type cls::var
#define SVAL_IMPL_NS_INIT(type, ns, cls, var, init) \
    type cls::var = init
#define GVAL_DECL(type, var) \
    extern type var
#define GVAL_IMPL(type, var) \
    type var
#define GVAL_IMPL_INIT(type, var, init) \
    type var = init
#define GARY_DECL(type, var, size) \
    extern type var[size]
#define GARY_IMPL(type, var, size) \
    type var[size]
#define PTR_HOST_TO_TADDR(host) ((TADDR)(host))
#define PTR_HOST_INT_TO_TADDR(host) ((TADDR)(host))
#define VPTR_HOST_VTABLE_TO_TADDR(host) ((TADDR)(host))
#define PTR_HOST_MEMBER_TADDR(type, host, memb) ((TADDR)&(host)->memb)
#define PTR_HOST_INT_MEMBER_TADDR(type, host, memb) ((TADDR)&(host)->memb)
#define PTR_TO_MEMBER_TADDR(type, ptr, memb) ((TADDR)&((ptr)->memb))
#define PTR_READ(addr, size) ((PVOID)(addr))

#define PTR_NULL NULL

#define DAC_EMPTY()
#define DAC_EMPTY_ERR()
#define DAC_EMPTY_RET(retVal)
#define DAC_UNEXPECTED() 

#define DACCOP_IGNORE(warningCode, reasonString)

#endif // #ifdef DACCESS_COMPILE

//----------------------------------------------------------------------------
// dac_cast
// Casting utility, to be used for casting one class pointer type to another.
// Use as you would use static_cast
//
// dac_cast is designed to act just as static_cast does when
// dealing with pointers and their DAC abstractions. Specifically,
// it handles these coversions:
//
//      dac_cast<TargetType>(SourceTypeVal)
//
// where TargetType <- SourceTypeVal are
//
//      ?PTR(Tgt) <- TADDR     - Create PTR type (DPtr etc.) from TADDR
//      ?PTR(Tgt) <- ?PTR(Src) - Convert one PTR type to another
//      ?PTR(Tgt) <- Src *     - Create PTR type from dac host object instance
//      TADDR <- ?PTR(Src)     - Get TADDR of PTR object (DPtr etc.) 
//      TADDR <- Src *         - Get TADDR of dac host object instance 
//
// Note that there is no direct convertion to other host-pointer types (because we don't
// know if you want a DPTR or VPTR etc.).  However, due to the implicit DAC conversions,
// you can just use dac_cast<PTR_Foo> and assign that to a Foo*.
// 
// The beauty of this syntax is that it is consistent regardless
// of source and target casting types. You just use dac_cast
// and the partial template specialization will do the right thing.
//
// One important thing to realise is that all "Foo *" types are
// assumed to be pointers to host instances that were marshalled by DAC.  This should
// fail at runtime if it's not the case.
//
// Some examples would be:
//
//   - Host pointer of one type to a related host pointer of another
//     type, i.e., MethodDesc * <-> InstantiatedMethodDesc *
//     Syntax: with MethodDesc *pMD, InstantiatedMethodDesc *pInstMD
//             pInstMd = dac_cast<PTR_InstantiatedMethodDesc>(pMD)
//             pMD = dac_cast<PTR_MethodDesc>(pInstMD)
//
//   - (D|V)PTR of one encapsulated pointer type to a (D|V)PTR of
//     another type, i.e., PTR_AppDomain <-> PTR_BaseDomain
//     Syntax: with PTR_AppDomain pAD, PTR_BaseDomain pBD
//             dac_cast<PTR_AppDomain>(pBD)
//             dac_cast<PTR_BaseDomain>(pAD)
//
// Example comparsions of some old and new syntax, where
//    h is a host pointer, such as "Foo *h;"
//    p is a DPTR, such as "PTR_Foo p;"
// 
//      PTR_HOST_TO_TADDR(h)           ==> dac_cast<TADDR>(h)
//      PTR_TO_TADDR(p)                ==> dac_cast<TADDR>(p)
//      PTR_Foo(PTR_HOST_TO_TADDR(h))  ==> dac_cast<PTR_Foo>(h)
//
//----------------------------------------------------------------------------
template <typename Tgt, typename Src>
inline Tgt dac_cast(Src src)
{
#ifdef DACCESS_COMPILE
    // In DAC builds, first get a TADDR for the source, then create the
    // appropriate destination instance.
    TADDR addr = dac_imp::getTaddr(src);
    return dac_imp::makeDacInst<Tgt>::fromTaddr(addr);
#else
    // In non-DAC builds, dac_cast is the same as a C-style cast because we need to support:
    //  - casting away const
    //  - conversions between pointers and TADDR
    // Perhaps we should more precisely restrict it's usage, but we get the precise 
    // restrictions in DAC builds, so it wouldn't buy us much.
    return (Tgt)(src);
#endif
}

//----------------------------------------------------------------------------
//
// Convenience macros which work for either mode.
//
//----------------------------------------------------------------------------

#define SPTR_DECL(type, var) _SPTR_DECL(type*, PTR_##type, var)
#define SPTR_IMPL(type, cls, var) _SPTR_IMPL(type*, PTR_##type, cls, var)
#define SPTR_IMPL_INIT(type, cls, var, init) _SPTR_IMPL_INIT(type*, PTR_##type, cls, var, init)
#define SPTR_IMPL_NS(type, ns, cls, var) _SPTR_IMPL_NS(type*, PTR_##type, ns, cls, var)
#define SPTR_IMPL_NS_INIT(type, ns, cls, var, init) _SPTR_IMPL_NS_INIT(type*, PTR_##type, ns, cls, var, init)
#define GPTR_DECL(type, var) _GPTR_DECL(type*, PTR_##type, var)
#define GPTR_IMPL(type, var) _GPTR_IMPL(type*, PTR_##type, var)
#define GPTR_IMPL_INIT(type, var, init) _GPTR_IMPL_INIT(type*, PTR_##type, var, init)


// If you want to marshal a single instance of an ArrayDPtr over to the host and
// return a pointer to it, you can use this function.  However, this is unsafe because
// users of value may assume they can do pointer arithmetic on it.  This is exactly
// the bugs ArrayDPtr is designed to prevent.  See code:__ArrayDPtr for details.
template<typename type>
inline type* DacUnsafeMarshalSingleElement( ArrayDPTR(type) arrayPtr )
{
    return (DPTR(type))(arrayPtr);
}

//----------------------------------------------------------------------------
//
// Forward typedefs for system types.  This is a convenient place
// to declare things for system types, plus it gives us a central
// place to look at when deciding what types may cause issues for
// cross-platform compilation.
//
//----------------------------------------------------------------------------

typedef ArrayDPTR(BYTE)    PTR_BYTE;
typedef DPTR(PTR_BYTE) PTR_PTR_BYTE;
typedef DPTR(PTR_PTR_BYTE) PTR_PTR_PTR_BYTE;
typedef ArrayDPTR(signed char) PTR_SBYTE;
typedef ArrayDPTR(const BYTE) PTR_CBYTE;
typedef DPTR(INT8)    PTR_INT8;
typedef DPTR(INT16)   PTR_INT16;
typedef DPTR(WORD)    PTR_WORD;
typedef DPTR(USHORT)  PTR_USHORT;
typedef DPTR(DWORD)   PTR_DWORD;
typedef DPTR(LONG)    PTR_LONG;
typedef DPTR(ULONG)   PTR_ULONG;
typedef DPTR(INT32)   PTR_INT32;
typedef DPTR(UINT32)  PTR_UINT32;
typedef DPTR(ULONG64) PTR_ULONG64;
typedef DPTR(INT64)   PTR_INT64;
typedef DPTR(UINT64)  PTR_UINT64;
typedef DPTR(SIZE_T)  PTR_SIZE_T;
typedef DPTR(size_t)  PTR_size_t;
typedef DPTR(TADDR)   PTR_TADDR;
typedef DPTR(int)     PTR_int;
typedef DPTR(BOOL)    PTR_BOOL;
typedef DPTR(unsigned) PTR_unsigned;

typedef S8PTR(char)           PTR_STR;
typedef S8PTR(const char)     PTR_CSTR;
typedef S8PTR(char)           PTR_UTF8;
typedef S8PTR(const char)     PTR_CUTF8;
typedef S16PTR(wchar_t)       PTR_WSTR;
typedef S16PTR(const wchar_t) PTR_CWSTR;

typedef DPTR(T_CONTEXT)                  PTR_CONTEXT;
typedef DPTR(PTR_CONTEXT)                PTR_PTR_CONTEXT;
typedef DPTR(struct _EXCEPTION_POINTERS) PTR_EXCEPTION_POINTERS;
typedef DPTR(struct _EXCEPTION_RECORD)   PTR_EXCEPTION_RECORD;

typedef DPTR(struct _EXCEPTION_REGISTRATION_RECORD) PTR_EXCEPTION_REGISTRATION_RECORD;

typedef DPTR(struct IMAGE_COR_VTABLEFIXUP) PTR_IMAGE_COR_VTABLEFIXUP;
typedef DPTR(IMAGE_DATA_DIRECTORY)  PTR_IMAGE_DATA_DIRECTORY;
typedef DPTR(IMAGE_DEBUG_DIRECTORY)  PTR_IMAGE_DEBUG_DIRECTORY;
typedef DPTR(IMAGE_DOS_HEADER)      PTR_IMAGE_DOS_HEADER;
typedef DPTR(IMAGE_NT_HEADERS)      PTR_IMAGE_NT_HEADERS;
typedef DPTR(IMAGE_NT_HEADERS32)    PTR_IMAGE_NT_HEADERS32;
typedef DPTR(IMAGE_NT_HEADERS64)    PTR_IMAGE_NT_HEADERS64;
typedef DPTR(IMAGE_SECTION_HEADER)  PTR_IMAGE_SECTION_HEADER;
typedef DPTR(IMAGE_TLS_DIRECTORY)   PTR_IMAGE_TLS_DIRECTORY;

#if defined(DACCESS_COMPILE)
#include <corhdr.h>
#include <clrdata.h>
#include <xclrdata.h>
#endif

#ifdef _WIN64
typedef DPTR(T_RUNTIME_FUNCTION) PTR_RUNTIME_FUNCTION;
typedef DPTR(struct _UNWIND_INFO)      PTR_UNWIND_INFO;
#if defined(_TARGET_AMD64_)
typedef DPTR(union _UNWIND_CODE)       PTR_UNWIND_CODE;
#endif // _TARGET_AMD64_
#endif // _WIN64

#ifdef _TARGET_ARM_
typedef DPTR(RUNTIME_FUNCTION) PTR_RUNTIME_FUNCTION;
#endif

//----------------------------------------------------------------------------
//
// A PCODE is a valid PC/IP value -- a pointer to an instruction, possibly including some processor mode bits.
// (On ARM, for example, a PCODE value should should have the low-order THUMB_CODE bit set if the code should
// be executed in that mode.)
//
typedef TADDR PCODE;
typedef DPTR(PCODE) PTR_PCODE;
typedef DPTR(PTR_PCODE) PTR_PTR_PCODE;

// There is another concept we should have, "pointer to the start of an instruction" -- a PCODE with any mode bits masked off.  
// Attempts to introduce this concept, and classify uses of PCODE as one or the other,
// turned out to be too hard: either name choice required *many* code changes, and decisions in unfamiliar code.  So despite the
// the comment above, the PCODE is currently sometimes used for the PINSTR concept.

// See PCODEToPINSTR in utilcode.h for conversion from PCODE to PINSTR.

//----------------------------------------------------------------------------
//
// The access code compile must compile data structures that exactly
// match the real structures for access to work.  The access code
// doesn't want all of the debugging validation code, though, so
// distinguish between _DEBUG, for declaring general debugging data
// and always-on debug code, and _DEBUG_IMPL, for debugging code
// which will be disabled when compiling for external access.
//
//----------------------------------------------------------------------------

#if !defined(_DEBUG_IMPL) && defined(_DEBUG) && !defined(DACCESS_COMPILE)
#define _DEBUG_IMPL 1
#endif

// Helper macro for tracking EnumMemoryRegions progress.
#if 0
#define EMEM_OUT(args) DacWarning args
#else
#define EMEM_OUT(args)
#endif

// Macros like MAIN_CLR_MODULE_NAME* for the DAC module
#ifdef FEATURE_MAIN_CLR_MODULE_USES_CORE_NAME
#define MAIN_DAC_MODULE_NAME_W  W("mscordaccore")
#define MAIN_DAC_MODULE_DLL_NAME_W  W("mscordaccore.dll")
#else
#define MAIN_DAC_MODULE_NAME_W  W("mscordacwks")
#define MAIN_DAC_MODULE_DLL_NAME_W  W("mscordacwks.dll")
#endif

// TARGET_CONSISTENCY_CHECK represents a condition that should not fail unless the DAC target is corrupt. 
// This is in contrast to ASSERTs in DAC infrastructure code which shouldn't fail regardless of the memory
// read from the target.  At the moment we treat these the same, but in the future we will want a mechanism
// for disabling just the target consistency checks (eg. for tests that intentionally use corrupted targets).
// @dbgtodo : Separating asserts and target consistency checks is tracked by DevDiv Bugs 31674
#define TARGET_CONSISTENCY_CHECK(expr,msg) _ASSERTE_MSG(expr,msg)

#endif // #ifndef __daccess_h__
