// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

/******************************************************************
*                                                                 *
*  intsafe.h -- This module defines helper functions to prevent   *
*               integer overflow issues.                          *
*                                                                 *
*  Copyright (c) Microsoft Corp.  All rights reserved.            *
*                                                                 *
*                                                                 *

*                                                                 *
******************************************************************/
#ifndef _INTSAFE_H_INCLUDED_
#define _INTSAFE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <specstrings.h>    // for __in, etc.

#if defined(__GNUC__)

#define __in
#define __out

#endif // !__GNUC__


#if defined(_AMD64_)
#ifdef __cplusplus
extern "C" {
#endif
#define UnsignedMultiply128 _umul128
ULONG64
UnsignedMultiply128 (
    __in ULONG64  Multiplier,
    __in ULONG64  Multiplicand,
    __out ULONG64 *HighProduct
    );
#ifdef _MSC_VER
#pragma intrinsic(_umul128)
#endif // _MSC_VER
#ifdef __cplusplus
}
#endif
#endif // _AMD64_

#ifndef FEATURE_PAL

#ifdef  _WIN64
typedef unsigned __int64    size_t;
typedef unsigned __int64    UINT_PTR;
typedef unsigned __int64    ULONG_PTR;
typedef unsigned __int64    DWORD_PTR;
typedef unsigned __int64    SIZE_T;
#else
typedef __w64 unsigned int  size_t;
typedef __w64 unsigned int  UINT_PTR;
typedef __w64 unsigned long ULONG_PTR;
typedef __w64 unsigned long DWORD_PTR;
typedef __w64 unsigned long SIZE_T;
#endif
typedef          char       CHAR;
typedef          int        INT;
typedef          long       LONG;
typedef unsigned char       UCHAR;
typedef unsigned short      USHORT;
typedef unsigned short      WORD;
typedef unsigned int        UINT;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef unsigned __int64    ULONGLONG;


typedef LONG HRESULT;

#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#endif

#ifndef FAILED
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif

#define S_OK ((HRESULT)0x00000000L)

#endif // !FEATURE_PAL

#define INTSAFE_E_ARITHMETIC_OVERFLOW       ((HRESULT)0x80070216L)  // 0x216 = 534 = ERROR_ARITHMETIC_OVERFLOW

#ifndef LOWORD
#define LOWORD(l)       ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#endif

#ifndef HIWORD
#define HIWORD(l)       ((WORD)(((DWORD_PTR)(l)) >> 16))
#endif

#define HIDWORD(_qw)    ((ULONG)((_qw) >> 32))
#define LODWORD(_qw)    ((ULONG)(_qw))

#if defined(MIDL_PASS) || defined(RC_INVOKED) || defined(_M_CEE_PURE) \
    || defined(_68K_) || defined(_MPPC_) || defined(_PPC_)            \
    || defined(_M_IA64) || defined(_M_AMD64)

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) ((unsigned __int64)((ULONG)(a)) * (unsigned __int64)((ULONG)(b)))
#endif

#elif defined(_M_IX86)

#ifndef UInt32x32To64
#define UInt32x32To64(a, b) (unsigned __int64)((unsigned __int64)(ULONG)(a) * (ULONG)(b))
#endif

#else

#error Must define a target architecture.

#endif

#define INT_MAX         2147483647
#define LONG_MAX        2147483647L
#define USHRT_MAX       0xffff
#define UINT_MAX        0xffffffff
#define ULONG_MAX       0xffffffffUL
#define DWORD_MAX       0xffffffffUL

//
// It is common for -1 to be used as an error value for various types
//
#define USHORT_ERROR    (0xffff)
#define INT_ERROR       (-1)
#define LONG_ERROR      (-1L)
#define UINT_ERROR      (0xffffffff)
#define ULONG_ERROR     (0xffffffffUL)
#ifdef _MSC_VER
#define ULONGLONG_ERROR (0xffffffffffffffffui64)
#define HIDWORD_MASK (0xffffffff00000000ui64)
#else // _MSC_VER
#define ULONGLONG_ERROR (0xffffffffffffffffULL)
#define HIDWORD_MASK (0xffffffff00000000ULL)
#endif // _MSC_VER
#ifdef _WIN64
#define SIZET_ERROR     ULONGLONG_ERROR
#else
#define SIZET_ERROR     ULONG_ERROR
#endif

//
// We make some assumptions about the sizes of various types. Let's be
// explicit about those assumptions and check them.
//
C_ASSERT(sizeof(unsigned short) == 2);
C_ASSERT(sizeof(unsigned int) == 4);
C_ASSERT(sizeof(ULONG) == 4);

//
// INT -> signed char conversion
//
__inline
HRESULT
IntToSignedChar(
    __in INT iOperand,
    __out signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((iOperand >= -128) && (iOperand <= 127))
    {
        *pch = (signed char)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> UCHAR conversion
//
__inline
HRESULT
IntToUChar(
    __in INT iOperand,
    __out UCHAR* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((iOperand >= 0) && (iOperand <= 255))
    {
        *pch = (UCHAR)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// LONG -> UCHAR conversion
//
__inline
HRESULT
LongToUChar(
    __in LONG lOperand,
    __out UCHAR* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if ((lOperand >= 0) && (lOperand <= 255))
    {
        *pch = (UCHAR)lOperand;
        hr = S_OK;
    }

    return hr;
}

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// INT -> CHAR conversion
//
__forceinline
HRESULT
IntToChar(
    __in INT iOperand,
    __out CHAR* pch)
{
#ifdef _CHAR_UNSIGNED
    return IntToUChar(iOperand, (UCHAR*)pch);
#else
    return IntToSignedChar(iOperand, (signed char*)pch);
#endif
}

//
// INT -> USHORT conversion
//
__inline
HRESULT
IntToUShort(
    __in INT iOperand,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if ((iOperand >= 0) && (iOperand <= USHRT_MAX))
    {
        *pusResult = (USHORT)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> UINT conversion
//
__inline
HRESULT
IntToUInt(
    __in INT iOperand,
    __out UINT* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (iOperand >= 0)
    {
        *puResult = (UINT)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> ULONG conversion
//
__inline
HRESULT
IntToULong(
    __in INT iOperand,
    __out ULONG* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (iOperand >= 0)
    {
        *pulResult = (ULONG)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// INT -> ULONGLONG conversion
//
__inline
HRESULT
IntToULongLong(
    __in INT iOperand,
    __out ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONG_ERROR;

    if (iOperand >= 0)
    {
        *pullResult = (ULONGLONG)iOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> signed char conversion
//
__inline
HRESULT
UIntToSignedChar(
    __in UINT uOperand,
    __out signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (uOperand <= 127)
    {
        *pch = (signed char)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> UCHAR conversion
//
__inline
HRESULT
UIntToUChar(
    __in UINT uOperand,
    __out UCHAR* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (uOperand <= 255)
    {
        *pch = (UCHAR)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> BYTE conversion
//
#define UIntToByte   UIntToUChar

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// UINT -> CHAR conversion
//
__forceinline
HRESULT
UIntToChar(
    __in UINT uOperand,
    __out CHAR* pch)
{
#ifdef _CHAR_UNSIGNED
    return UIntToUChar(uOperand, (UCHAR*)pch);
#else
    return UIntToSignedChar(uOperand, (signed char*)pch);
#endif // _CHAR_UNSIGNED
}

//
// UINT -> INT conversion
//
__inline
HRESULT
UIntToInt(
    __in UINT uOperand,
    __out INT* piResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *piResult = INT_ERROR;

    if (uOperand <= INT_MAX)
    {
        *piResult = (INT)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// UINT -> LONG conversion
//
__inline
HRESULT
UIntToLong(
    __in UINT Operand,
    __out LONG* Result)
{
    if (Operand <= LONG_MAX)
    {
        *Result = (LONG)Operand;
        return S_OK;
    }
    else
    {
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// UINT -> ULONG conversion
//
__inline
HRESULT
UIntToULong(
    __in UINT uOperand,
    __out ULONG* pulResult)
{
    *pulResult = (ULONG)uOperand;
    
    return S_OK;
}

//
// ULONG -> UCHAR conversion
//
__inline
HRESULT
ULongToSignedChar(
    __in ULONG ulOperand,
    __out signed char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (ulOperand <= 127)
    {
        *pch = (signed char)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// ULONG -> UCHAR conversion
//
__inline
HRESULT
ULongToUChar(
    __in ULONG ulOperand,
    __out unsigned char* pch)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pch = 0;

    if (ulOperand <= 255)
    {
        *pch = (unsigned char)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// __inline is not sufficient. __forceinline is necessary.
// If the function is not inlined and you link .objs compiled with different compiler switches,
// you get one or the other function arbitrarily chosen.
//
// ULONG -> CHAR conversion
//
__forceinline
HRESULT
ULongToChar(
    __in ULONG ulOperand,
    __out CHAR* pch)
{
#ifdef _CHAR_UNSIGNED
    return ULongToUChar(ulOperand, (unsigned char*)pch);
#else
    return ULongToSignedChar(ulOperand, (signed char*)pch);
#endif // _CHAR_UNSIGNED
}

//
// ULONG -> USHORT conversion
//
__inline
HRESULT
ULongToUShort(
    __in ULONG ulOperand,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (ulOperand <= USHRT_MAX)
    {
        *pusResult = (USHORT)ulOperand;
        hr = S_OK;
    }

    return hr;
}

//
// ULONG -> INT conversion
//
__inline
HRESULT
ULongToInt(
    __in ULONG ulOperand,
    __out INT* piResult)
{
    if (ulOperand <= INT_MAX)
    {
        *piResult = (INT)ulOperand;
        return S_OK;
    }
    else
    {
        *piResult = INT_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// ULONG -> UINT conversion
//
__inline
HRESULT
ULongToUInt(
    __in ULONG ulOperand,
    __out UINT* puResult)
{
    *puResult = (UINT)ulOperand;
    
    return S_OK;
}

//
// ULONG -> LONG conversion
//
__inline
HRESULT
ULongToLong(
    __in ULONG Operand,
    __out LONG* Result)
{
    if (Operand <= LONG_MAX)
    {
        *Result = (LONG)Operand;
        return S_OK;
    }
    else
    {
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// ULONGLONG -> INT conversion
//
__inline
HRESULT
ULongLongToInt(
    __in ULONGLONG ullOperand,
    __out INT* piResult)
{
    if (ullOperand <= INT_MAX)
    {
        *piResult = (INT)ullOperand;
        return S_OK;
    }
    else
    {
        *piResult = INT_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// ULONGLONG -> LONG conversion
//
__inline
HRESULT
ULongLongToLong(
    __in ULONGLONG Operand,
    __out LONG* Result)
{
    if (Operand <= LONG_MAX)
    {
        *Result = (LONG)Operand;
        return S_OK;
    }
    else
    {
        *Result = LONG_ERROR;
        return INTSAFE_E_ARITHMETIC_OVERFLOW;
    }
}

//
// UINT -> USHORT conversion
//
__inline
HRESULT
UIntToUShort(
    __in UINT uOperand,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (uOperand <= USHRT_MAX)
    {
        *pusResult = (USHORT)uOperand;
        hr = S_OK;
    }

    return hr;
}

//
// ULONGLONG -> USHORT conversion
//
__inline
HRESULT
ULongLongToUShort(
    __in ULONGLONG ullOperand,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    USHORT usResult = USHORT_ERROR;

    if (ullOperand <= USHRT_MAX)
    {
        usResult = (USHORT)ullOperand;
        hr = S_OK;
    }
    *pusResult = usResult;

    return hr;
}

//
// ULONGLONG -> ULONG conversion
//
__inline
HRESULT
ULongLongToULong(
    __in ULONGLONG ullOperand,
    __out ULONG* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;
    
    if (ullOperand <= ULONG_MAX)
    {
        *pulResult = (ULONG)ullOperand;
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR -> ULONG conversion
// ULONG_PTR -> ULONG conversion
//
#ifdef _WIN64

#define UIntPtrToULong  ULongLongToULong
#define ULongPtrToULong ULongLongToULong

#else

__inline
HRESULT
UIntPtrToULong(
    __in UINT_PTR Operand,
    __out ULONG* pResult)
{
    *pResult = (ULONG)Operand;
    return S_OK;
}

__inline
HRESULT
ULongPtrToULong(
    __in ULONG_PTR Operand,
    __out ULONG* pResult)
{
    *pResult = (ULONG)Operand;
    return S_OK;
}

#endif

//
// ULONGLONG -> UINT conversion
//
__inline
HRESULT
ULongLongToUInt(
    __in ULONGLONG ullOperand,
    __out UINT* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;
    
    if (ullOperand <= UINT_MAX)
    {
        *puResult = (UINT)ullOperand;
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR -> UINT conversion
// ULONG_PTR -> UINT conversion
//
#ifdef _WIN64

#define UIntPtrToUInt  ULongLongToUInt
#define ULongPtrToUInt ULongLongToUInt

#else

__inline
HRESULT
UIntPtrToUInt(
    __in UINT_PTR Operand,
    __out UINT* pResult)
{
    *pResult = (UINT)Operand;
    return S_OK;
}

__inline
HRESULT
ULongPtrToUInt(
    __in ULONG_PTR Operand,
    __out UINT* pResult)
{
    *pResult = (UINT)Operand;
    return S_OK;
}

#endif

//
// * -> BYTE conversion (BYTE is always unsigned char)
//
#define  IntToByte   IntToUChar
#define UIntToByte  UIntToUChar
#define  LongToByte  LongToUChar
#define ULongToByte ULongToUChar

//
// * -> WORD conversion (WORD is always unsigned short)
//
#define IntToWord               IntToUShort
#define LongToWord              LongToUShort
#define LongLongToWord          LongLongToUShort
#define UIntToWord              UIntToUShort
#define ULongToWord             ULongToUShort
#define ULongLongToWord         ULongLongToUShort
#define UIntPtrToWord           UIntPtrToUShort
#define ULongPtrToWord          ULongPtrToUShort
#define SizeTToWord             SizeTToUShort
#define SIZETToWord             SIZETToUShort

//
// WORD -> * conversion (WORD is always unsigned short)
//
#define WordToUChar             UShortToUChar
#define WordToByte              UShortToByte
#define WordToChar              UShortToChar
#define WordToSignedChar        UShortToSignedChar
#define WordToInt               UShortToInt
#define WordToLong              UShortToLong
#define WordToLongLong          UShortToLongLong
#define WordToIntPtr            UShortToIntPtr
#define WordToLongPtr           UShortToLongPtr

//
// * -> DWORD conversion (DWORD is always ULONG)
//
#define CharToDWord             CharToULong
#define SignedCharToDWord       SignedCharToULong
#define ShortToDWord            ShortToULong
#define IntToDWord              IntToULong
#define LongToDWord             LongToULong
#define LongLongToDWord         LongLongToULong
#define UIntToDWord             UIntToULong
#define ULongLongToDWord        ULongLongToULong
#define IntPtrToDWord           IntPtrToULong
#define LongPtrToDWord          LongPtrToULong
#define UIntPtrToDWord          UIntPtrToULong
#define ULongPtrToDWord         ULongPtrToULong
#define SizeTToDWord            SizeTToULong
#define SIZETToDWord            SIZETToULong

//
// DWORD -> * conversion (DWORD is always ULONG)
//
#define DWordToChar             ULongToChar
#define DWordToUChar            ULongToUChar
#define DWordToByte             ULongToByte
#define DWordToSignedChar       ULongToSignedChar
#define DWordToUShort           ULongToUShort
#define DWordToUInt             ULongToUInt
#define DWordToInt              ULongToInt
#define DWordToLong             ULongToLong
#define DWordToLongLong         ULongToLongLong
#define DWordToIntPtr           ULongToIntPtr
#define DWordToLongPtr          ULongToLongPtr


//
// * -> UINT_PTR conversion (UINT_PTR is UINT on Win32, ULONGLONG on Win64)
//
#ifdef _WIN64
#define CharToUIntPtr           CharToULongLong
#define SignedCharToUIntPtr     SignedCharToULongLong
#define ShortToUIntPtr          ShortToULongLong
#define IntToUIntPtr            IntToULongLong
#define LongToUIntPtr           LongToULongLong
#define LongLongToUIntPtr       LongLongToULongLong
#define IntPtrToUIntPtr         IntPtrToULongLong
#define LongPtrToUIntPtr        LongPtrToULongLong
#else
#define CharToUIntPtr           CharToUInt
#define SignedCharToUIntPtr     SignedCharToUInt
#define ShortToUIntPtr          ShortToUInt

__inline
HRESULT
IntToUIntPtr(
    __in INT iOperand,
    __out UINT_PTR* puResult)
{
	return IntToUInt(iOperand, (UINT*)puResult);
}

#define LongToUIntPtr           LongToUInt
#define LongLongToUIntPtr       LongLongToUInt

#define IntPtrToUIntPtr         IntPtrToUInt
#define LongPtrToUIntPtr        LongPtrToUInt
#endif

__inline
HRESULT
ULongLongToUIntPtr(
    __in ULONGLONG ullOperand,
    __out UINT_PTR* puResult)
{
#ifdef _WIN64
	*puResult = ullOperand;
	return S_OK;
#else
	return ULongLongToUInt(ullOperand, (UINT*)puResult);
#endif
}


//
// UINT_PTR -> * conversion (UINT_PTR is UINT on Win32, ULONGLONG on Win64)
//
#ifdef _WIN64
#define UIntPtrToUShort         ULongLongToUShort
#define UIntPtrToInt            ULongLongToInt
#define UIntPtrToLong           ULongLongToLong
#define UIntPtrToLongLong       ULongLongToLongLong
#define UIntPtrToIntPtr         ULongLongToIntPtr
#define UIntPtrToLongPtr        ULongLongToLongPtr
#else

__inline
HRESULT
UIntPtrToUShort(
    __in UINT_PTR uOperand,
    __out USHORT* pusResult)
{
	return UIntToUShort((UINT)uOperand, pusResult);
}

__inline
HRESULT
UIntPtrToInt(
    __in UINT_PTR uOperand,
    __out INT* piResult)
{
	return UIntToInt((UINT)uOperand, piResult);
}

__inline
HRESULT
UIntPtrToLong(
    __in UINT_PTR Operand,
    __out LONG* Result)
{
	return UIntToLong((UINT)Operand, Result);
}

#define UIntPtrToLongLong       UIntToLongLong
#define UIntPtrToIntPtr         UIntToIntPtr
#define UIntPtrToLongPtr        UIntToLongPtr
#endif


//
// * -> ULONG_PTR conversion (ULONG_PTR is ULONG on Win32, ULONGLONG on Win64)
//
#ifdef _WIN64
#define CharToULongPtr          CharToULongLong
#define SignedCharToULongPtr    SignedCharToULongLong
#define ShortToULongPtr         ShortToULongLong
#define IntToULongPtr           IntToULongLong
#define LongToULongPtr          LongToULongLong
#define LongLongToULongPtr      LongLongToULongLong
#define IntPtrToULongPtr        IntPtrToULongLong
#define LongPtrToULongPtr       LongPtrToULongLong
#else
#define CharToULongPtr          CharToULong
#define SignedCharToULongPtr    SignedCharToULong
#define ShortToULongPtr         ShortToULong

__inline
HRESULT
IntToULongPtr(
    __in INT iOperand,
    __out ULONG_PTR* pulResult)
{
	return IntToULong(iOperand, (ULONG*)pulResult);
}

#define LongToULongPtr          LongToULong
#define LongLongToULongPtr      LongLongToULong

#define IntPtrToULongPtr        IntPtrToULong
#define LongPtrToULongPtr       LongPtrToULong
#endif

__inline
HRESULT
ULongLongToULongPtr(
    __in ULONGLONG ullOperand,
    __out ULONG_PTR* pulResult)
{
#ifdef _WIN64
	*pulResult = ullOperand;
	return S_OK;
#else
	return ULongLongToULong(ullOperand, (ULONG*)pulResult);
#endif
}


//
// ULONG_PTR -> * conversion (ULONG_PTR is ULONG on Win32, ULONGLONG on Win64)
//
#ifdef _WIN64
#define ULongPtrToUShort        ULongLongToUShort
#define ULongPtrToInt           ULongLongToInt
#define ULongPtrToLong          ULongLongToLong
#define ULongPtrToLongLong      ULongLongToLongLong
#define ULongPtrToIntPtr        ULongLongToIntPtr
#define ULongPtrToLongPtr       ULongLongToLongPtr
#else

__inline
HRESULT
ULongPtrToUShort(
    __in ULONG_PTR ulOperand,
    __out USHORT* pusResult)
{
	return ULongToUShort((ULONG)ulOperand, pusResult);
}

__inline
HRESULT
ULongPtrToInt(
    __in ULONG_PTR ulOperand,
    __out INT* piResult)
{
	return ULongToInt((ULONG)ulOperand, piResult);
}

__inline
HRESULT
ULongPtrToLong(
    __in ULONG_PTR Operand,
    __out LONG* Result)
{
	return ULongToLong((ULONG)Operand, Result);
}

#define ULongPtrToLongLong      ULongToLongLong
#define ULongPtrToIntPtr        ULongToIntPtr
#define ULongPtrToLongPtr       ULongToLongPtr
#endif

//
// * -> size_t conversion (size_t is always UINT_PTR)
//
#define CharToSizeT             CharToUIntPtr
#define SignedCharToSizeT       SignedCharToUIntPtr
#define ShortToSizeT            ShortToUIntPtr
#define IntToSizeT              IntToUIntPtr
#define LongToSizeT             LongToUIntPtr
#define LongLongToSizeT         LongLongToUIntPtr
#define ULongLongToSizeT        ULongLongToUIntPtr
#define IntPtrToSizeT           IntPtrToUIntPtr
#define LongPtrToSizeT          LongPtrToUIntPtr

//
// size_t -> * conversion (size_t is always UINT_PTR)
//
#define SizeTToUShort           UIntPtrToUShort
#define SizeTToUInt             UIntPtrToUInt
#define SizeTToULong            UIntPtrToULong
#define SizeTToInt              UIntPtrToInt
#define SizeTToLong             UIntPtrToLong
#define SizeTToLongLong         UIntPtrToLongLong
#define SizeTToIntPtr           UIntPtrToIntPtr
#define SizeTToLongPtr          UIntPtrToLongPtr

//
// * -> SIZE_T conversion (SIZE_T is always ULONG_PTR)
//
#define CharToSIZET             CharToULongPtr
#define SignedCharToSIZET       SignedCharToULongPtr
#define ShortToSIZET            ShortToULongPtr
#define IntToSIZET              IntToULongPtr
#define LongToSIZET             LongToULongPtr
#define LongLongToSIZET         LongLongToULongPtr
#define IntPtrToSIZET           IntPtrToULongPtr
#define LongPtrToSIZET          LongPtrToULongPtr
#define ULongLongToSIZET        ULongLongToULongPtr

//
// SIZE_T -> * conversion (SIZE_T is always ULONG_PTR)
//
#define SIZETToUShort           ULongPtrToUShort
#define SIZETToUInt             ULongPtrToUInt
#define SIZETToULong            ULongPtrToULong
#define SIZETToUIntPtr          ULongPtrToUIntPtr
#define SIZETToULongPtr         ULongPtrToULongPtr
#define SIZETToInt              ULongPtrToInt
#define SIZETToLong             ULongPtrToLong
#define SIZETToLongLong         ULongPtrToLongLong
#define SIZETToIntPtr           ULongPtrToIntPtr
#define SIZETToLongPtr          ULongPtrToLongPtr

//
// * -> DWORD_PTR conversion (DWORD_PTR is always ULONG_PTR)
//
#define CharToDWordPtr             CharToULongPtr
#define SignedCharToDWordPtr       SignedCharToULongPtr
#define ShortToDWordPtr            ShortToULongPtr
#define IntToDWordPtr              IntToULongPtr
#define LongToDWordPtr             LongToULongPtr
#define LongLongToDWordPtr         LongLongToULongPtr
#define ULongLongToDWordPtr        ULongLongToULongPtr
#define IntPtrToDWordPtr           IntPtrToULongPtr
#define LongPtrToDWordPtr          LongPtrToULongPtr

//
// DWORD_PTR -> * conversion (DWORD_PTR is always ULONG_PTR)
//
#define DWordPtrToUShort           ULongPtrToUShort
#define DWordPtrToUInt             ULongPtrToUInt
#define DWordPtrToULong            ULongPtrToULong
#define DWordPtrToDWord            ULongPtrToDWord
#define DWordPtrToInt              ULongPtrToInt
#define DWordPtrToLong             ULongPtrToLong
#define DWordPtrToLongLong         ULongPtrToLongLong
#define DWordPtrToIntPtr           ULongPtrToIntPtr
#define DWordPtrToLongPtr          ULongPtrToLongPtr

//
// USHORT addition
//
__inline
HRESULT
UShortAdd(
    __in USHORT usAugend,
    __in USHORT usAddend,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (((USHORT)(usAugend + usAddend)) >= usAugend)
    {
        *pusResult = (usAugend + usAddend);
        hr = S_OK;
    }
    
    return hr;
}

//
// WORD addtition
//
#define WordAdd     UShortAdd

//
// UINT addition
//
__inline
HRESULT
UIntAdd(
    __in UINT uAugend,
    __in UINT uAddend,
    __out UINT* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if ((uAugend + uAddend) >= uAugend)
    {
        *puResult = (uAugend + uAddend);
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR addition
//
#define UIntPtrAdd		SizeTAdd

//
// ULONG addition
//
__inline
HRESULT
ULongAdd(
    __in ULONG ulAugend,
    __in ULONG ulAddend,
    __out ULONG* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if ((ulAugend + ulAddend) >= ulAugend)
    {
        *pulResult = (ulAugend + ulAddend);
        hr = S_OK;
    }
    
    return hr;
}

//
// ULONG_PTR addition
//
#ifdef _WIN64
#define ULongPtrAdd     ULongLongAdd
#else
__inline
HRESULT
ULongPtrAdd(
    __in ULONG_PTR ulAugend,
    __in ULONG_PTR ulAddend,
    __out ULONG_PTR* pulResult)
{
	return ULongAdd((ULONG)ulAugend, (ULONG)ulAddend, (ULONG*)pulResult);
}
#endif // _WIN64

//
// DWORD addition
//
#define DWordAdd        ULongAdd

//
// DWORD_PTR addition
//
#define DWordPtrAdd		ULongPtrAdd

//
// size_t addition
//
__inline
HRESULT
SizeTAdd(
    __in size_t Augend,
    __in size_t Addend,
    __out size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if ((Augend + Addend) >= Augend)
    {
        *pResult = (Augend + Addend);
        hr = S_OK;
    }
    
    return hr;
}

//
// SIZE_T addition
//
#define SIZETAdd	ULongPtrAdd

//
// ULONGLONG addition
//
__inline
HRESULT
ULongLongAdd(
    __in ULONGLONG ullAugend,
    __in ULONGLONG ullAddend,
    __out ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if ((ullAugend + ullAddend) >= ullAugend)
    {
        *pullResult = (ullAugend + ullAddend);
        hr = S_OK;
    }
    
    return hr;
}

//
// USHORT subtraction
//
__inline
HRESULT
UShortSub(
    __in USHORT usMinuend,
    __in USHORT usSubtrahend,
    __out USHORT* pusResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pusResult = USHORT_ERROR;

    if (usMinuend >= usSubtrahend)
    {
        *pusResult = (usMinuend - usSubtrahend);
        hr = S_OK;
    }
    
    return hr;
}

//
// WORD subtraction
//
#define WordSub     UShortSub


//
// UINT subtraction
//
__inline
HRESULT
UIntSub(
    __in UINT uMinuend,
    __in UINT uSubtrahend,
    __out UINT* puResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *puResult = UINT_ERROR;

    if (uMinuend >= uSubtrahend)
    {
        *puResult = (uMinuend - uSubtrahend);
        hr = S_OK;
    }
    
    return hr;
}

//
// UINT_PTR subtraction
//
#define UIntPtrSub	SizeTSub

//
// ULONG subtraction
//
__inline
HRESULT
ULongSub(
    __in ULONG ulMinuend,
    __in ULONG ulSubtrahend,
    __out ULONG* pulResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pulResult = ULONG_ERROR;

    if (ulMinuend >= ulSubtrahend)
    {
        *pulResult = (ulMinuend - ulSubtrahend);
        hr = S_OK;
    }
    
    return hr;
}

//
// ULONG_PTR subtraction
//
#ifdef _WIN64
#define ULongPtrSub ULongLongSub
#else
__inline
HRESULT
ULongPtrSub(
    __in ULONG_PTR ulMinuend,
    __in ULONG_PTR ulSubtrahend,
    __out ULONG_PTR* pulResult)
{
	return ULongSub((ULONG)ulMinuend, (ULONG)ulSubtrahend, (ULONG*)pulResult);
}
#endif // _WIN64


//
// DWORD subtraction
//
#define DWordSub        ULongSub

//
// DWORD_PTR subtraction
//
#define DWordPtrSub		ULongPtrSub

//
// size_t subtraction
//
__inline
HRESULT
SizeTSub(
    __in size_t Minuend,
    __in size_t Subtrahend,
    __out size_t* pResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pResult = SIZET_ERROR;

    if (Minuend >= Subtrahend)
    {
        *pResult = (Minuend - Subtrahend);
        hr = S_OK;
    }
    
    return hr;
}

//
// SIZE_T subtraction
//
#define SIZETSub	ULongPtrSub

//
// ULONGLONG subtraction
//
__inline
HRESULT
ULongLongSub(
    __in ULONGLONG ullMinuend,
    __in ULONGLONG ullSubtrahend,
    __out ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
    *pullResult = ULONGLONG_ERROR;

    if (ullMinuend >= ullSubtrahend)
    {
        *pullResult = (ullMinuend - ullSubtrahend);
        hr = S_OK;
    }
    
    return hr;
}

//
// USHORT multiplication
//
__inline
HRESULT
UShortMult(
    __in USHORT usMultiplicand,
    __in USHORT usMultiplier,
    __out USHORT* pusResult)
{
    ULONG ulResult = ((ULONG)usMultiplicand) * (ULONG)usMultiplier;
    
    return ULongToUShort(ulResult, pusResult);
}

//
// WORD multiplication
//
#define WordMult      UShortMult

//
// UINT multiplication
//
__inline
HRESULT
UIntMult(
    __in UINT uMultiplicand,
    __in UINT uMultiplier,
    __out UINT* puResult)
{
    ULONGLONG ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

    return ULongLongToUInt(ull64Result, puResult);
}

//
// UINT_PTR multiplication
//
#ifdef _WIN64
#define UIntPtrMult     ULongLongMult
#else
__inline
HRESULT
UIntPtrMult(
    __in UINT_PTR ulMultiplicand,
    __in UINT_PTR ulMultiplier,
    __out UINT_PTR* pulResult)
{
	return UIntMult((UINT)ulMultiplicand, (UINT)ulMultiplier, (UINT*)pulResult);
}
#endif // _WIN64

//
// ULONG multiplication
//
__inline
HRESULT
ULongMult(
    __in ULONG ulMultiplicand,
    __in ULONG ulMultiplier,
    __out ULONG* pulResult)
{
    ULONGLONG ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);
    
    return ULongLongToULong(ull64Result, pulResult);
}

//
// ULONG_PTR multiplication
//
#ifdef _WIN64
#define ULongPtrMult    ULongLongMult
#else
__inline
HRESULT
ULongPtrMult(
    __in ULONG_PTR ulMultiplicand,
    __in ULONG_PTR ulMultiplier,
    __out ULONG_PTR* pulResult)
{
	return ULongMult((ULONG)ulMultiplicand, (ULONG)ulMultiplier, (ULONG*)pulResult);
}
#endif // _WIN64


//
// DWORD multiplication
//
#define DWordMult       ULongMult

//
// DWORD_PTR multiplication
//
#define DWordPtrMult	ULongPtrMult

//
// size_t multiplication
//
#define SizeTMult		UIntPtrMult

//
// SIZE_T multiplication
//
#define SIZETMult		ULongPtrMult

//
// ULONGLONG multiplication
//
__inline
HRESULT
ULongLongMult(
    __in ULONGLONG ullMultiplicand,
    __in ULONGLONG ullMultiplier,
    __out ULONGLONG* pullResult)
{
    HRESULT hr = INTSAFE_E_ARITHMETIC_OVERFLOW;
#ifdef _AMD64_
    ULONGLONG u64ResultHigh;
    ULONGLONG u64ResultLow;
    
    *pullResult = ULONGLONG_ERROR;
    
    u64ResultLow = UnsignedMultiply128(ullMultiplicand, ullMultiplier, &u64ResultHigh);
    if (u64ResultHigh == 0)
    {
        *pullResult = u64ResultLow;
        hr = S_OK;
    }
#else
    // 64x64 into 128 is like 32.32 x 32.32.
    //
    // a.b * c.d = a*(c.d) + .b*(c.d) = a*c + a*.d + .b*c + .b*.d
    // back in non-decimal notation where A=a*2^32 and C=c*2^32:  
    // A*C + A*d + b*C + b*d
    // So there are four components to add together.
    //   result = (a*c*2^64) + (a*d*2^32) + (b*c*2^32) + (b*d)
    //
    // a * c must be 0 or there would be bits in the high 64-bits
    // a * d must be less than 2^32 or there would be bits in the high 64-bits
    // b * c must be less than 2^32 or there would be bits in the high 64-bits
    // then there must be no overflow of the resulting values summed up.
    
    ULONG dw_a;
    ULONG dw_b;
    ULONG dw_c;
    ULONG dw_d;
    ULONGLONG ad = 0;
    ULONGLONG bc = 0;
    ULONGLONG bd = 0;
    ULONGLONG ullResult = 0;
    
    *pullResult = ULONGLONG_ERROR;

    dw_a = (ULONG)(ullMultiplicand >> 32);
    dw_c = (ULONG)(ullMultiplier >> 32);

    // common case -- if high dwords are both zero, no chance for overflow
    if ((dw_a == 0) && (dw_c == 0))
    {
        dw_b = (DWORD)ullMultiplicand;
        dw_d = (DWORD)ullMultiplier;

        *pullResult = (((ULONGLONG)dw_b) * (ULONGLONG)dw_d);
        hr = S_OK;
    }
    else
    {
        // a * c must be 0 or there would be bits set in the high 64-bits
        if ((dw_a == 0) ||
            (dw_c == 0))
        {
            dw_d = (DWORD)ullMultiplier;

            // a * d must be less than 2^32 or there would be bits set in the high 64-bits
            ad = (((ULONGLONG)dw_a) * (ULONGLONG)dw_d);
            if ((ad & HIDWORD_MASK) == 0)
            {
                dw_b = (DWORD)ullMultiplicand;

                // b * c must be less than 2^32 or there would be bits set in the high 64-bits
                bc = (((ULONGLONG)dw_b) * (ULONGLONG)dw_c);
                if ((bc & HIDWORD_MASK) == 0)
                {
                    // now sum them all up checking for overflow.
                    // shifting is safe because we already checked for overflow above
                    if (SUCCEEDED(ULongLongAdd(bc << 32, ad << 32, &ullResult)))                        
                    {
                        // b * d
                        bd = (((ULONGLONG)dw_b) * (ULONGLONG)dw_d);
                    
                        if (SUCCEEDED(ULongLongAdd(ullResult, bd, &ullResult)))
                        {
                            *pullResult = ullResult;
                            hr = S_OK;
                        }
                    }
                }
            }
        }
    }
#endif // _AMD64_  
    
    return hr;
}

#endif // _INTSAFE_H_INCLUDED_
