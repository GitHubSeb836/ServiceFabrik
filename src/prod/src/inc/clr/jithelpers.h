// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#ifndef DYNAMICJITHELPER
//I should never try to generate an alignment stub for a dynamic helper
#define DYNAMICJITHELPER(code,fn,sig) JITHELPER(code,fn,sig)
#endif


// This file now also associates jit helpers and Mdil helpers (last column)
// Unless JITHELPER1 or DYNAMICJITHELPER1 are defined, this information is
// completely filtered out.
// MDILHelpers.h defines for each MDIL helper the expected signature, non-retail
// builds check that an MDIL helper's signature matches the corresponding 
// jit helper's signature.
//
// See the definition of CorInfoHelpFunc in corinfo.h for more details.
//
#ifndef DYNAMICJITHELPER1
#define DYNAMICJITHELPER1(code,fn,sig,mdilhelper) DYNAMICJITHELPER(code,fn,sig)
#endif

// PuCLR Integration note: I [maxmg] saw we changed JITHELPER macro to JITHELPER1 in order to map
// the MDIL helpers, it seems to me we can rename the macro names to JITHELPER again and merge
// both macros into one.

#ifndef JITHELPER1
#define JITHELPER1(code,fn,sig,mdilhelper) JITHELPER(code,fn,sig)
#endif

// pfnHelper is set to NULL if it is a stubbed helper.
// It will be set in InitJITHelpers2

    JITHELPER1(CORINFO_HELP_UNDEF,               NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_UNDEF)

    // Arithmetic
    JITHELPER1(CORINFO_HELP_DIV,                 JIT_Div,            CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DIV)
    JITHELPER1(CORINFO_HELP_MOD,                 JIT_Mod,            CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_MOD)
    JITHELPER1(CORINFO_HELP_UDIV,                JIT_UDiv,           CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_UDIV)
    JITHELPER1(CORINFO_HELP_UMOD,                JIT_UMod,           CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_UMOD)

    // CORINFO_HELP_DBL2INT, CORINFO_HELP_DBL2UINT, and CORINFO_HELP_DBL2LONG get
    // patched for CPUs that support SSE2 (P4 and above).
#if !defined(_WIN64)
    JITHELPER1(CORINFO_HELP_LLSH,                JIT_LLsh,           CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_LLSH)
    JITHELPER1(CORINFO_HELP_LRSH,                JIT_LRsh,           CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_LRSH)
    JITHELPER1(CORINFO_HELP_LRSZ,                JIT_LRsz,           CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_LRSZ)
#else
    JITHELPER1(CORINFO_HELP_LLSH,                NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_LLSH)
    JITHELPER1(CORINFO_HELP_LRSH,                NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_LRSH)
    JITHELPER1(CORINFO_HELP_LRSZ,                NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_LRSZ)
#endif
    JITHELPER1(CORINFO_HELP_LMUL,                JIT_LMul,           CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_LMUL)
    JITHELPER1(CORINFO_HELP_LMUL_OVF,            JIT_LMulOvf,        CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_LMUL_OVF)
    JITHELPER1(CORINFO_HELP_ULMUL_OVF,           JIT_ULMulOvf,       CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_ULMUL_OVF)
    JITHELPER1(CORINFO_HELP_LDIV,                JIT_LDiv,           CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_LDIV)
    JITHELPER1(CORINFO_HELP_LMOD,                JIT_LMod,           CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_LMOD)
    JITHELPER1(CORINFO_HELP_ULDIV,               JIT_ULDiv,          CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_ULDIV)
    JITHELPER1(CORINFO_HELP_ULMOD,               JIT_ULMod,          CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_ULMOD)
    JITHELPER1(CORINFO_HELP_LNG2DBL,             JIT_Lng2Dbl,        CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_LNG2DBL)
    JITHELPER1(CORINFO_HELP_ULNG2DBL,            JIT_ULng2Dbl,       CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_ULNG2DBL)
    DYNAMICJITHELPER1(CORINFO_HELP_DBL2INT,      JIT_Dbl2Lng,        CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2INT) /* use long version */
    JITHELPER1(CORINFO_HELP_DBL2INT_OVF,         JIT_Dbl2IntOvf,     CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2INT_OVF)
    DYNAMICJITHELPER1(CORINFO_HELP_DBL2LNG,      JIT_Dbl2Lng,        CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2LNG)
    JITHELPER1(CORINFO_HELP_DBL2LNG_OVF,         JIT_Dbl2LngOvf,     CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2LNG_OVF)
    DYNAMICJITHELPER1(CORINFO_HELP_DBL2UINT,     JIT_Dbl2Lng,        CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2UINT) /* use long version */
    JITHELPER1(CORINFO_HELP_DBL2UINT_OVF,        JIT_Dbl2UIntOvf,    CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2UINT_OVF)
    JITHELPER1(CORINFO_HELP_DBL2ULNG,            JIT_Dbl2ULng,       CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2ULNG)
    JITHELPER1(CORINFO_HELP_DBL2ULNG_OVF,        JIT_Dbl2ULngOvf,    CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_DBL2ULNG_OVF)
    JITHELPER1(CORINFO_HELP_FLTREM,              JIT_FltRem,         CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_FLTREM)
    JITHELPER1(CORINFO_HELP_DBLREM,              JIT_DblRem,         CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_DBLREM)
    JITHELPER1(CORINFO_HELP_FLTROUND,            JIT_FloatRound,     CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_FLTROUND)
    JITHELPER1(CORINFO_HELP_DBLROUND,            JIT_DoubleRound,    CORINFO_HELP_SIG_16_STACK,              MDIL_HELP_DBLROUND)

    // Allocating a new object
#ifdef FEATURE_REMOTING
    JITHELPER1(CORINFO_HELP_NEW_CROSSCONTEXT,            JIT_NewCrossContext,    CORINFO_HELP_SIG_REG_ONLY,  MDIL_HELP_NEW_CROSSCONTEXT)
#else
    JITHELPER1(CORINFO_HELP_NEW_CROSSCONTEXT,            NULL,       CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_NEW_CROSSCONTEXT)
#endif
    JITHELPER1(CORINFO_HELP_NEWFAST,                     JIT_New,    CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_NEWFAST)
    DYNAMICJITHELPER1(CORINFO_HELP_NEWSFAST,             JIT_New,    CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_NEWSFAST)
    DYNAMICJITHELPER1(CORINFO_HELP_NEWSFAST_ALIGN8,      JIT_New,    CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_NEWSFAST_ALIGN8)
    JITHELPER1(CORINFO_HELP_NEW_MDARR,                   JIT_NewMDArr,CORINFO_HELP_SIG_8_VA,                 MDIL_HELP_NEW_MDARR)
    JITHELPER1(CORINFO_HELP_NEWARR_1_DIRECT,             JIT_NewArr1,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_NEWARR_1_OBJ,         JIT_NewArr1,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_NEWARR_1_VC,          JIT_NewArr1,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_NEWARR_1_ALIGN8,      JIT_NewArr1,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)

    JITHELPER1(CORINFO_HELP_STRCNS,              JIT_StrCns,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_STRCNS)
// Any new jit helpers need to be removed for the RYUJIT_CTPBUILD so we can build a 4.5.1 compatible JIT
#if !defined(RYUJIT_CTPBUILD) 
    JITHELPER1(CORINFO_HELP_STRCNS_CURRENT_MODULE, NULL,             CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
#endif

    // Object model
    JITHELPER1(CORINFO_HELP_INITCLASS,           JIT_InitClass,      CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_INITCLASS)
    JITHELPER1(CORINFO_HELP_INITINSTCLASS,       JIT_InitInstantiatedClass, CORINFO_HELP_SIG_REG_ONLY,       MDIL_HELP_INITINSTCLASS)

    JITHELPER1(CORINFO_HELP_ISINSTANCEOFINTERFACE,JIT_IsInstanceOfInterface, CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_ISINSTANCEOFINTERFACE)
    JITHELPER1(CORINFO_HELP_ISINSTANCEOFARRAY,   JIT_IsInstanceOfArray,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_ISINSTANCEOFARRAY)
    JITHELPER1(CORINFO_HELP_ISINSTANCEOFCLASS,   JIT_IsInstanceOfClass,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_ISINSTANCEOFCLASS)
    JITHELPER1(CORINFO_HELP_ISINSTANCEOFANY,     JIT_IsInstanceOfAny,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)

    JITHELPER1(CORINFO_HELP_CHKCASTINTERFACE,    JIT_ChkCastInterface,CORINFO_HELP_SIG_REG_ONLY,             MDIL_HELP_CHKCASTINTERFACE)
    JITHELPER1(CORINFO_HELP_CHKCASTARRAY,        JIT_ChkCastArray,   CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_CHKCASTARRAY)
    JITHELPER1(CORINFO_HELP_CHKCASTCLASS,        JIT_ChkCastClass,   CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_CHKCASTCLASS)
    JITHELPER1(CORINFO_HELP_CHKCASTANY,          JIT_ChkCastAny,     CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)

    JITHELPER1(CORINFO_HELP_CHKCASTCLASS_SPECIAL,JIT_ChkCastClassSpecial,CORINFO_HELP_SIG_REG_ONLY,          MDIL_HELP_CHKCASTCLASS_SPECIAL)
    DYNAMICJITHELPER1(CORINFO_HELP_BOX,          JIT_Box,            CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_BOX_NULLABLE,        JIT_Box,            CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_UNBOX,               JIT_Unbox,          CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_UNBOX_NULLABLE,      JIT_Unbox_Nullable, CORINFO_HELP_SIG_4_STACK,               MDIL_HELP_UNBOX_NULLABLE)   
    JITHELPER1(CORINFO_HELP_GETREFANY,           JIT_GetRefAny,      CORINFO_HELP_SIG_8_STACK,               MDIL_HELP_GETREFANY)
#if defined(_TARGET_ARM_)
    DYNAMICJITHELPER1(CORINFO_HELP_ARRADDR_ST,   JIT_Stelem_Ref,     CORINFO_HELP_SIG_4_STACK,               MDIL_HELP_ARRADDR_ST)
#else
    JITHELPER1(CORINFO_HELP_ARRADDR_ST,          JIT_Stelem_Ref,     CORINFO_HELP_SIG_4_STACK,               MDIL_HELP_ARRADDR_ST)
#endif // _TARGET_ARM_
    JITHELPER1(CORINFO_HELP_LDELEMA_REF,         JIT_Ldelema_Ref,    CORINFO_HELP_SIG_4_STACK,               MDIL_HELP_LDELEMA_REF)

    // Exceptions
    JITHELPER1(CORINFO_HELP_THROW,               IL_Throw,           CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_RETHROW,             IL_Rethrow,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_USER_BREAKPOINT,     JIT_UserBreakpoint, CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_USER_BREAKPOINT)
    JITHELPER1(CORINFO_HELP_RNGCHKFAIL,          JIT_RngChkFail,     CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_RNGCHKFAIL)
    JITHELPER1(CORINFO_HELP_OVERFLOW,            JIT_Overflow,       CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_OVERFLOW)
    JITHELPER1(CORINFO_HELP_THROWDIVZERO,        JIT_ThrowDivZero,   CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_INTERNALTHROW,       JIT_InternalThrow,  CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_INTERNALTHROW)
    JITHELPER1(CORINFO_HELP_VERIFICATION,        IL_VerificationError,CORINFO_HELP_SIG_REG_ONLY,             MDIL_HELP_VERIFICATION)
    JITHELPER1(CORINFO_HELP_SEC_UNMGDCODE_EXCPT, JIT_SecurityUnmanagedCodeException, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_SEC_UNMGDCODE_EXCPT)
    JITHELPER1(CORINFO_HELP_FAIL_FAST,           JIT_FailFast,       CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_FAIL_FAST)
    JITHELPER1(CORINFO_HELP_METHOD_ACCESS_EXCEPTION,JIT_ThrowMethodAccessException, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_METHOD_ACCESS_EXCEPTION)
    JITHELPER1(CORINFO_HELP_FIELD_ACCESS_EXCEPTION,JIT_ThrowFieldAccessException, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_FIELD_ACCESS_EXCEPTION)
    JITHELPER1(CORINFO_HELP_CLASS_ACCESS_EXCEPTION,JIT_ThrowClassAccessException, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_CLASS_ACCESS_EXCEPTION)

#ifdef WIN64EXCEPTIONS
    JITHELPER1(CORINFO_HELP_ENDCATCH,            NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_ENDCATCH)
#else
    JITHELPER1(CORINFO_HELP_ENDCATCH,            JIT_EndCatch,       CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_ENDCATCH)
#endif

#ifdef _TARGET_AMD64_
    DYNAMICJITHELPER1(CORINFO_HELP_MON_ENTER,        JIT_MonEnterWorker, CORINFO_HELP_SIG_REG_ONLY,          MDIL_HELP_MON_ENTER)
    DYNAMICJITHELPER1(CORINFO_HELP_MON_EXIT,         JIT_MonExitWorker, CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_MON_EXIT)
    DYNAMICJITHELPER1(CORINFO_HELP_MON_ENTER_STATIC, JIT_MonEnterStatic,CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_MON_ENTER_STATIC)
    DYNAMICJITHELPER1(CORINFO_HELP_MON_EXIT_STATIC,  JIT_MonExitStatic,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_MON_EXIT_STATIC)
#else
    JITHELPER1(CORINFO_HELP_MON_ENTER,               JIT_MonEnterWorker, CORINFO_HELP_SIG_REG_ONLY,          MDIL_HELP_MON_ENTER)
    JITHELPER1(CORINFO_HELP_MON_EXIT,                JIT_MonExitWorker, CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_MON_EXIT)
    JITHELPER1(CORINFO_HELP_MON_ENTER_STATIC,        JIT_MonEnterStatic,CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_MON_ENTER_STATIC)
    JITHELPER1(CORINFO_HELP_MON_EXIT_STATIC,         JIT_MonExitStatic,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_MON_EXIT_STATIC)
#endif

    JITHELPER1(CORINFO_HELP_GETCLASSFROMMETHODPARAM, JIT_GetClassFromMethodParam, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_GETCLASSFROMMETHODPARAM)
    JITHELPER1(CORINFO_HELP_GETSYNCFROMCLASSHANDLE,  JIT_GetSyncFromClassHandle, CORINFO_HELP_SIG_REG_ONLY,  MDIL_HELP_GETSYNCFROMCLASSHANDLE)

    // Security callout support 
    JITHELPER1(CORINFO_HELP_SECURITY_PROLOG,         JIT_Security_Prolog,CORINFO_HELP_SIG_REG_ONLY,          MDIL_HELP_SECURITY_PROLOG)
    JITHELPER1(CORINFO_HELP_SECURITY_PROLOG_FRAMED,  JIT_Security_Prolog_Framed,CORINFO_HELP_SIG_REG_ONLY,   MDIL_HELP_SECURITY_PROLOG_FRAMED)

    JITHELPER1(CORINFO_HELP_METHOD_ACCESS_CHECK,     JIT_MethodAccessCheck,CORINFO_HELP_SIG_8_STACK,         MDIL_HELP_METHOD_ACCESS_CHECK)
    JITHELPER1(CORINFO_HELP_FIELD_ACCESS_CHECK,      JIT_FieldAccessCheck,CORINFO_HELP_SIG_4_STACK,          MDIL_HELP_FIELD_ACCESS_CHECK)
    JITHELPER1(CORINFO_HELP_CLASS_ACCESS_CHECK,      JIT_ClassAccessCheck,CORINFO_HELP_SIG_4_STACK,          MDIL_HELP_CLASS_ACCESS_CHECK)

    JITHELPER1(CORINFO_HELP_DELEGATE_SECURITY_CHECK, JIT_DelegateSecurityCheck,CORINFO_HELP_SIG_REG_ONLY,    MDIL_HELP_DELEGATE_SECURITY_CHECK)

     // Verification runtime callout support 
    JITHELPER1(CORINFO_HELP_VERIFICATION_RUNTIME_CHECK, JIT_VerificationRuntimeCheck,CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_VERIFICATION_RUNTIME_CHECK)

    // GC support
    JITHELPER1(CORINFO_HELP_STOP_FOR_GC,         JIT_RareDisableHelper,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_STOP_FOR_GC)
#ifdef ENABLE_FAST_GCPOLL_HELPER
    DYNAMICJITHELPER1(CORINFO_HELP_POLL_GC,      JIT_PollGC,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_POLL_GC)
#else
    JITHELPER1(CORINFO_HELP_POLL_GC,             JIT_PollGC,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_POLL_GC)
#endif
    JITHELPER1(CORINFO_HELP_STRESS_GC,           JIT_StressGC,       CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_STRESS_GC)

    JITHELPER1(CORINFO_HELP_CHECK_OBJ,           JIT_CheckObj,       CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_CHECK_OBJ)

    // GC Write barrier support
    DYNAMICJITHELPER1(CORINFO_HELP_ASSIGN_REF,   JIT_WriteBarrier,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_ASSIGN_REF)
    DYNAMICJITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF, JIT_CheckedWriteBarrier,CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_CHECKED_ASSIGN_REF)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_ENSURE_NONHEAP, JIT_WriteBarrierEnsureNonHeapTarget,CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_ASSIGN_REF_ENSURE_NONHEAP)

    DYNAMICJITHELPER1(CORINFO_HELP_ASSIGN_BYREF, JIT_ByRefWriteBarrier,CORINFO_HELP_SIG_NO_ALIGN_STUB,       MDIL_HELP_ASSIGN_BYREF)

    JITHELPER1(CORINFO_HELP_ASSIGN_STRUCT,       JIT_StructWriteBarrier,CORINFO_HELP_SIG_4_STACK,            MDIL_HELP_ASSIGN_STRUCT)

    // Accessing fields
    JITHELPER1(CORINFO_HELP_GETFIELD8,                   JIT_GetField8,CORINFO_HELP_SIG_REG_ONLY,            MDIL_HELP_GETFIELD8)
    JITHELPER1(CORINFO_HELP_SETFIELD8,                   JIT_SetField8,CORINFO_HELP_SIG_4_STACK,             MDIL_HELP_SETFIELD8)
    JITHELPER1(CORINFO_HELP_GETFIELD16,                  JIT_GetField16,CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_GETFIELD16)
    JITHELPER1(CORINFO_HELP_SETFIELD16,                  JIT_SetField16,CORINFO_HELP_SIG_4_STACK,            MDIL_HELP_SETFIELD16)
    JITHELPER1(CORINFO_HELP_GETFIELD32,                  JIT_GetField32,CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_GETFIELD32)
    JITHELPER1(CORINFO_HELP_SETFIELD32,                  JIT_SetField32,CORINFO_HELP_SIG_4_STACK,            MDIL_HELP_SETFIELD32)
    JITHELPER1(CORINFO_HELP_GETFIELD64,                  JIT_GetField64,CORINFO_HELP_SIG_REG_ONLY,           MDIL_HELP_GETFIELD64)
    JITHELPER1(CORINFO_HELP_SETFIELD64,                  JIT_SetField64,CORINFO_HELP_SIG_8_STACK,            MDIL_HELP_SETFIELD64)
    JITHELPER1(CORINFO_HELP_GETFIELDOBJ,                 JIT_GetFieldObj,CORINFO_HELP_SIG_REG_ONLY,          MDIL_HELP_GETFIELDOBJ)
    JITHELPER1(CORINFO_HELP_SETFIELDOBJ,                 JIT_SetFieldObj,CORINFO_HELP_SIG_4_STACK,           MDIL_HELP_SETFIELDOBJ)
    JITHELPER1(CORINFO_HELP_GETFIELDSTRUCT,              JIT_GetFieldStruct,CORINFO_HELP_SIG_8_STACK,        MDIL_HELP_GETFIELDSTRUCT)
    JITHELPER1(CORINFO_HELP_SETFIELDSTRUCT,              JIT_SetFieldStruct,CORINFO_HELP_SIG_8_STACK,        MDIL_HELP_SETFIELDSTRUCT)
    JITHELPER1(CORINFO_HELP_GETFIELDFLOAT,               JIT_GetFieldFloat,CORINFO_HELP_SIG_REG_ONLY,        MDIL_HELP_GETFIELDFLOAT)
    JITHELPER1(CORINFO_HELP_SETFIELDFLOAT,               JIT_SetFieldFloat,CORINFO_HELP_SIG_4_STACK,         MDIL_HELP_SETFIELDFLOAT)
    JITHELPER1(CORINFO_HELP_GETFIELDDOUBLE,              JIT_GetFieldDouble,CORINFO_HELP_SIG_REG_ONLY,       MDIL_HELP_GETFIELDDOUBLE) 
    JITHELPER1(CORINFO_HELP_SETFIELDDOUBLE,              JIT_SetFieldDouble,CORINFO_HELP_SIG_8_STACK,        MDIL_HELP_SETFIELDDOUBLE)

    JITHELPER1(CORINFO_HELP_GETFIELDADDR,                JIT_GetFieldAddr,CORINFO_HELP_SIG_REG_ONLY,         MDIL_HELP_GETFIELDADDR)

#ifdef FEATURE_REMOTING
    JITHELPER1(CORINFO_HELP_GETSTATICFIELDADDR_CONTEXT,  JIT_GetStaticFieldAddr_Context,CORINFO_HELP_SIG_REG_ONLY,  MDIL_HELP_GETSTATICFIELDADDR_CONTEXT)
#else
    JITHELPER1(CORINFO_HELP_GETSTATICFIELDADDR_CONTEXT,  NULL,       CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB,        MDIL_HELP_GETSTATICFIELDADDR_CONTEXT)
#endif
#ifdef FEATURE_MIXEDMODE
    // TLS
    JITHELPER1(CORINFO_HELP_GETSTATICFIELDADDR_TLS,      JIT_GetStaticFieldAddr_Tls,CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_GETSTATICFIELDADDR_TLS)
#else // FEATURE_MIXEDMODE
    JITHELPER1(CORINFO_HELP_GETSTATICFIELDADDR_TLS,      NULL,       CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB,        MDIL_HELP_GETSTATICFIELDADDR_TLS)
#endif // FEATURE_MIXEDMODE

    JITHELPER1(CORINFO_HELP_GETGENERICS_GCSTATIC_BASE,   JIT_GetGenericsGCStaticBase,CORINFO_HELP_SIG_REG_ONLY,     MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETGENERICS_NONGCSTATIC_BASE, JIT_GetGenericsNonGCStaticBase,CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)

#if defined(_TARGET_X86_)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_GCSTATIC_BASE,          NULL, CORINFO_HELP_SIG_REG_ONLY,        MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_NONGCSTATIC_BASE,       NULL, CORINFO_HELP_SIG_REG_ONLY,        MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_GCSTATIC_BASE_NOCTOR,   NULL, CORINFO_HELP_SIG_REG_ONLY,        MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_NONGCSTATIC_BASE_NOCTOR,NULL, CORINFO_HELP_SIG_REG_ONLY,        MDIL_HELP_UNDEF)
#else
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_GCSTATIC_BASE,          JIT_GetSharedGCStaticBase, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB,          MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_NONGCSTATIC_BASE,       JIT_GetSharedNonGCStaticBase, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB,       MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_GCSTATIC_BASE_NOCTOR,   JIT_GetSharedGCStaticBaseNoCtor, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB,    MDIL_HELP_UNDEF)
    DYNAMICJITHELPER1(CORINFO_HELP_GETSHARED_NONGCSTATIC_BASE_NOCTOR,JIT_GetSharedNonGCStaticBaseNoCtor, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_UNDEF)
#endif
    JITHELPER1(CORINFO_HELP_GETSHARED_GCSTATIC_BASE_DYNAMICCLASS,    JIT_GetSharedGCStaticBaseDynamicClass,CORINFO_HELP_SIG_REG_ONLY,    MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_NONGCSTATIC_BASE_DYNAMICCLASS, JIT_GetSharedNonGCStaticBaseDynamicClass,CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_CLASSINIT_SHARED_DYNAMICCLASS,           JIT_ClassInitDynamicClass,CORINFO_HELP_SIG_REG_ONLY,                MDIL_HELP_UNDEF)

    // Thread statics
    JITHELPER1(CORINFO_HELP_GETGENERICS_GCTHREADSTATIC_BASE,   JIT_GetGenericsGCThreadStaticBase,CORINFO_HELP_SIG_REG_ONLY,                           MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETGENERICS_NONGCTHREADSTATIC_BASE, JIT_GetGenericsNonGCThreadStaticBase,CORINFO_HELP_SIG_REG_ONLY,                       MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_GCTHREADSTATIC_BASE,                 JIT_GetSharedGCThreadStaticBase, CORINFO_HELP_SIG_REG_ONLY,                MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_NONGCTHREADSTATIC_BASE,              JIT_GetSharedNonGCThreadStaticBase, CORINFO_HELP_SIG_REG_ONLY,             MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_GCTHREADSTATIC_BASE_NOCTOR,          JIT_GetSharedGCThreadStaticBase, CORINFO_HELP_SIG_REG_ONLY,                MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_NONGCTHREADSTATIC_BASE_NOCTOR,       JIT_GetSharedNonGCThreadStaticBase, CORINFO_HELP_SIG_REG_ONLY,             MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_GCTHREADSTATIC_BASE_DYNAMICCLASS,    JIT_GetSharedGCThreadStaticBaseDynamicClass, CORINFO_HELP_SIG_REG_ONLY,    MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_GETSHARED_NONGCTHREADSTATIC_BASE_DYNAMICCLASS, JIT_GetSharedNonGCThreadStaticBaseDynamicClass, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)

    // Debugger
    JITHELPER1(CORINFO_HELP_DBG_IS_JUST_MY_CODE, JIT_DbgIsJustMyCode,CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_DBG_IS_JUST_MY_CODE)

    /* Profiling enter/leave probe addresses */
    DYNAMICJITHELPER1(CORINFO_HELP_PROF_FCN_ENTER,    JIT_ProfilerEnterLeaveTailcallStub, CORINFO_HELP_SIG_4_STACK, MDIL_HELP_PROF_FCN_ENTER)
    DYNAMICJITHELPER1(CORINFO_HELP_PROF_FCN_LEAVE,    JIT_ProfilerEnterLeaveTailcallStub, CORINFO_HELP_SIG_4_STACK, MDIL_HELP_PROF_FCN_LEAVE)
    DYNAMICJITHELPER1(CORINFO_HELP_PROF_FCN_TAILCALL, JIT_ProfilerEnterLeaveTailcallStub, CORINFO_HELP_SIG_4_STACK, MDIL_HELP_PROF_FCN_TAILCALL)

    // Miscellaneous
    JITHELPER1(CORINFO_HELP_BBT_FCN_ENTER,       JIT_LogMethodEnter,CORINFO_HELP_SIG_REG_ONLY,               MDIL_HELP_BBT_FCN_ENTER)

    JITHELPER1(CORINFO_HELP_PINVOKE_CALLI,       GenericPInvokeCalliHelper, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_PINVOKE_CALLI)

    JITHELPER1(CORINFO_HELP_TAILCALL,            JIT_TailCall,             CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_TAILCALL)

    JITHELPER1(CORINFO_HELP_GETCURRENTMANAGEDTHREADID,  JIT_GetCurrentManagedThreadId, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_GETCURRENTMANAGEDTHREADID)

#ifdef _WIN64
    // On Windows X64, the last argument is ignored.
    JITHELPER(CORINFO_HELP_INIT_PINVOKE_FRAME,  JIT_InitPInvokeFrame,  CORINFO_HELP_SIG_REG_ONLY)
#else
    DYNAMICJITHELPER1(CORINFO_HELP_INIT_PINVOKE_FRAME,  NULL,        CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_INIT_PINVOKE_FRAME)
#endif

#if defined(_WIN64) || defined(_TARGET_ARM_)

    JITHELPER1(CORINFO_HELP_MEMSET,              JIT_MemSet,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_MEMSET)
    JITHELPER1(CORINFO_HELP_MEMCPY,              JIT_MemCpy,         CORINFO_HELP_SIG_REG_ONLY,              MDIL_HELP_MEMCPY)

#else // _WIN64 || _TARGET_ARM_

    JITHELPER1(CORINFO_HELP_MEMSET,              NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_MEMSET)
    JITHELPER1(CORINFO_HELP_MEMCPY,              NULL,               CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_MEMCPY)

#endif

    // Generics
    JITHELPER1(CORINFO_HELP_RUNTIMEHANDLE_METHOD,    JIT_GenericHandleMethod,        CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_RUNTIMEHANDLE_METHOD)
    JITHELPER1(CORINFO_HELP_RUNTIMEHANDLE_METHOD_LOG,JIT_GenericHandleMethodLogging, CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_RUNTIMEHANDLE_METHOD_LOG)
    JITHELPER1(CORINFO_HELP_RUNTIMEHANDLE_CLASS,     JIT_GenericHandleClass,         CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_RUNTIMEHANDLE_CLASS)
    JITHELPER1(CORINFO_HELP_RUNTIMEHANDLE_CLASS_LOG, JIT_GenericHandleClassLogging,  CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_RUNTIMEHANDLE_CLASS_LOG)
#if defined(RYUJIT_CTPBUILD)
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE, JIT_GetRuntimeTypeHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE)
    // This helper is not needed for MDIL - MDIL does not support IL instructions that this helper is needed for
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE_MAYBENULL, JIT_GetRuntimeTypeHandle_MaybeNull, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_METHODDESC_TO_RUNTIMEMETHODHANDLE, JIT_GetRuntimeMethodHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_METHODDESC_TO_RUNTIMEMETHODHANDLE)
    JITHELPER1(CORINFO_HELP_FIELDDESC_TO_RUNTIMEFIELDHANDLE, JIT_GetRuntimeFieldHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_FIELDDESC_TO_RUNTIMEFIELDHANDLE)
#else // stuff below is only for post-CTP builds
#if defined(FEATURE_CORECLR) && defined(_TARGET_ARM_) // MDIL
    // These helpers are required for MDIL backward compatibility only. They are not used by current JITed code.
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE_OBSOLETE, JIT_GetRuntimeTypeHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE)
    JITHELPER1(CORINFO_HELP_METHODDESC_TO_RUNTIMEMETHODHANDLE_OBSOLETE, JIT_GetRuntimeMethodHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_METHODDESC_TO_RUNTIMEMETHODHANDLE)
    JITHELPER1(CORINFO_HELP_FIELDDESC_TO_RUNTIMEFIELDHANDLE_OBSOLETE, JIT_GetRuntimeFieldHandle, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_FIELDDESC_TO_RUNTIMEFIELDHANDLE)
#else 
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPEHANDLE_OBSOLETE, NULL, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_METHODDESC_TO_RUNTIMEMETHODHANDLE_OBSOLETE, NULL, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_FIELDDESC_TO_RUNTIMEFIELDHANDLE_OBSOLETE, NULL, CORINFO_HELP_SIG_CANNOT_USE_ALIGN_STUB, MDIL_HELP_UNDEF)
#endif // MDIL
#endif // defined(RYUJIT_CTBUILD)
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPE, JIT_GetRuntimeType,           CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_TYPEHANDLE_TO_RUNTIMETYPE)
    // This helper is not needed for MDIL - MDIL does not support IL instructions that this helper is needed for
    JITHELPER1(CORINFO_HELP_TYPEHANDLE_TO_RUNTIMETYPE_MAYBENULL, JIT_GetRuntimeType_MaybeNull, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_METHODDESC_TO_STUBRUNTIMEMETHOD, JIT_GetRuntimeMethodStub,CORINFO_HELP_SIG_REG_ONLY,     MDIL_HELP_METHODDESC_TO_STUBRUNTIMEMETHOD)
    JITHELPER1(CORINFO_HELP_FIELDDESC_TO_STUBRUNTIMEFIELD, JIT_GetRuntimeFieldStub,  CORINFO_HELP_SIG_REG_ONLY,      MDIL_HELP_FIELDDESC_TO_STUBRUNTIMEFIELD)

    JITHELPER1(CORINFO_HELP_VIRTUAL_FUNC_PTR,    JIT_VirtualFunctionPointer, CORINFO_HELP_SIG_4_STACK,       MDIL_HELP_VIRTUAL_FUNC_PTR)
    //JITHELPER1(CORINFO_HELP_VIRTUAL_FUNC_PTR_LOG,JIT_VirtualFunctionPointerLogging)

#ifndef RYUJIT_CTPBUILD
    JITHELPER1(CORINFO_HELP_READYTORUN_NEW,                NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_READYTORUN_NEWARR_1,           NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_READYTORUN_ISINSTANCEOF,       NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_READYTORUN_CHKCAST,            NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_READYTORUN_STATIC_BASE,        NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
    JITHELPER1(CORINFO_HELP_READYTORUN_VIRTUAL_FUNC_PTR,   NULL,   CORINFO_HELP_SIG_NO_ALIGN_STUB,         MDIL_HELP_UNDEF)
#endif

    JITHELPER1(CORINFO_HELP_EE_PRESTUB,          ThePreStub,                 CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_PRESTUB)

#if defined(_TARGET_X86_) || defined(_TARGET_AMD64_) || defined(_TARGET_ARM_)
    JITHELPER1(CORINFO_HELP_EE_PRECODE_FIXUP,    PrecodeFixupThunk,          CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_PRECODE_FIXUP)
#else
    JITHELPER1(CORINFO_HELP_EE_PRECODE_FIXUP,    NULL,                       CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_PRECODE_FIXUP)
#endif

    JITHELPER1(CORINFO_HELP_EE_PINVOKE_FIXUP,    NDirectImportThunk,         CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_PINVOKE_FIXUP)

    JITHELPER1(CORINFO_HELP_EE_VSD_FIXUP,        StubDispatchFixupStub,      CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_VSD_FIXUP)
    JITHELPER1(CORINFO_HELP_EE_EXTERNAL_FIXUP,   ExternalMethodFixupStub,    CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_EXTERNAL_FIXUP)
    JITHELPER1(CORINFO_HELP_EE_VTABLE_FIXUP,     VirtualMethodFixupStub,     CORINFO_HELP_SIG_NO_ALIGN_STUB, MDIL_HELP_EE_VTABLE_FIXUP)

#if (defined(_TARGET_X86_) || defined(_TARGET_ARM_)) && defined(FEATURE_REMOTING)
    JITHELPER1(CORINFO_HELP_EE_REMOTING_THUNK,   PrecodeRemotingThunk,       CORINFO_HELP_SIG_UNDEF,         MDIL_HELP_EE_REMOTING_THUNK)
#else
    JITHELPER1(CORINFO_HELP_EE_REMOTING_THUNK,   NULL,                       CORINFO_HELP_SIG_UNDEF,         MDIL_HELP_EE_REMOTING_THUNK)
#endif

// We do not need this to be saved in ngen images on Mac64 since the exception dispatch
// is not done via the OS and thus, there wont be any need to know this information
// by anyone.
#if !defined(_TARGET_X86_)
    JITHELPER1(CORINFO_HELP_EE_PERSONALITY_ROUTINE, ProcessCLRException,               CORINFO_HELP_SIG_UNDEF,   MDIL_HELP_EE_PERSONALITY_ROUTINE)
    JITHELPER1(CORINFO_HELP_EE_PERSONALITY_ROUTINE_FILTER_FUNCLET, ProcessCLRException,CORINFO_HELP_SIG_UNDEF,   MDIL_HELP_EE_PERSONALITY_ROUTINE_FILTER_FUNCLET)
#else
    JITHELPER1(CORINFO_HELP_EE_PERSONALITY_ROUTINE, NULL,                              CORINFO_HELP_SIG_UNDEF,   MDIL_HELP_EE_PERSONALITY_ROUTINE)
    JITHELPER1(CORINFO_HELP_EE_PERSONALITY_ROUTINE_FILTER_FUNCLET, NULL,               CORINFO_HELP_SIG_UNDEF,   MDIL_HELP_UNDEF      )
#endif

#ifdef _TARGET_X86_
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_EAX, JIT_WriteBarrierEAX, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_EAX)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_EBX, JIT_WriteBarrierEBX, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_EBX)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_ECX, JIT_WriteBarrierECX, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_ECX)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_ESI, JIT_WriteBarrierESI, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_ESI)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_EDI, JIT_WriteBarrierEDI, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_EDI)
    JITHELPER1(CORINFO_HELP_ASSIGN_REF_EBP, JIT_WriteBarrierEBP, CORINFO_HELP_SIG_NO_ALIGN_STUB,                 MDIL_HELP_ASSIGN_REF_EBP)

    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_EAX, JIT_CheckedWriteBarrierEAX, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_EAX)
    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_EBX, JIT_CheckedWriteBarrierEBX, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_EBX)
    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_ECX, JIT_CheckedWriteBarrierECX, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_ECX)
    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_ESI, JIT_CheckedWriteBarrierESI, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_ESI)
    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_EDI, JIT_CheckedWriteBarrierEDI, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_EDI)
    JITHELPER1(CORINFO_HELP_CHECKED_ASSIGN_REF_EBP, JIT_CheckedWriteBarrierEBP, CORINFO_HELP_SIG_NO_ALIGN_STUB,  MDIL_HELP_CHECKED_ASSIGN_REF_EBP)
#endif

#if defined(MDIL) && defined(_TARGET_ARM_)
    JITHELPER1(CORINFO_HELP_ALLOCA,          NULL, CORINFO_HELP_SIG_UNDEF,  MDIL_HELP_ALLOCA)
#endif //MDIL

// PuCLR Integration note:
// JbTodo:  This helper definition is missing it's MDIL helper counterpart.
    JITHELPER1(CORINFO_HELP_LOOP_CLONE_CHOICE_ADDR, JIT_LoopCloneChoiceAddr, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)
// JbTodo:  This helper definition is missing it's MDIL helper counterpart.
    JITHELPER1(CORINFO_HELP_DEBUG_LOG_LOOP_CLONING, JIT_DebugLogLoopCloning, CORINFO_HELP_SIG_REG_ONLY, MDIL_HELP_UNDEF)

#undef JITHELPER1
#undef DYNAMICJITHELPER1
#undef JITHELPER
#undef DYNAMICJITHELPER
