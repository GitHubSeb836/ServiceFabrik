// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#ifndef __VSASSERT_H__
#define __VSASSERT_H__

#ifndef FEATURE_PAL
#error "FEATURE_PAL must be defined for this file"
#else // FEATURE_PAL

#define VSASSERT(e, szMsg)                                      \
do {                                                            \
    if (!(e)) {                                                 \
        fprintf (stderr,                                        \
                 "ASSERT FAILED:\n"                             \
                 "\tExpression: %s\n"                           \
                 "\tLocation:   line %d in %s\n"                \
                 "\tFunction:   %s\n"                           \
                 "\tMessage:    %s\n",                          \
                 #e, __LINE__, __FILE__, __FUNCTION__, szMsg);  \
            DebugBreak();                                       \
    }                                                           \
} while (0)

#define VSFAIL(szMsg) VSASSERT(0, szMsg)
#define VSIMPLIES(fHypothesis, fConclusion, szMsg) VSASSERT(!(fHypothesis) || (fConclusion), szMsg)
#define VSVERIFY(fTest, szMsg) VSASSERT((fTest), (szMsg))

#undef VSAlloc
#undef VSAllocZero
#undef VSRealloc
#undef VSReallocZero
#undef VSFree
#undef VSSize
#undef VsDebAlloc
#undef VsDebRealloc
#undef VsDebSafeRealloc
#undef VsDebFree
#undef VsDebHeapSize

#undef VsDebHeapCreate
#undef VsDebHeapDestroy

#undef VsDebugInitialize
#undef VsDebugTerminate

// NOTE: These have changed to use the HeapAlloc family (as opposed to
// LocalAlloc family) because of HeapReAlloc's behavior (a block may move to
// satisfy a realloc request, as opposed to LocalReAlloc's behavior of simply
// failing).

#define VSAlloc(cb)          HeapAlloc(GetProcessHeap(), 0, cb)
#define VSAllocZero(cb)      HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cb)
#define VSRealloc(pv, cb)    HeapReAlloc(GetProcessHeap(), 0, pv, cb)
#define VSReallocZero(pv,cb) Rotors_pal_doesnt_have_vsrealloczero
#define VSFree(pv)           HeapFree(GetProcessHeap(), 0, pv)
#define VSSize(pv)           Rotors_pal_doesnt_have_heapsize

#define VsDebAlloc(flags,cb)          VSAlloc(cb)
#define VsDebRealloc(pv,flags,cb)     VSRealloc(pv,cb)
#define VsDebSafeRealloc(pv,flags,cb) Rotors_pal_doenst_have_saferealloc
#define VsDebFree(pv)                 VSFree(pv)
#define VsDebHeapSize(heap, pv)       VSSize(pv)

#define VsDebHeapCreate(flags, name)         Rotor_doesnt_have_heapcreate
#define VsDebHeapDestroy(heap, fLeakCheck)   Rotor_doesnt_have_heapdestroy

#define VsDebugAllocInternal(hheap,dwFlags,cb,pszFile,uLine,dwInst,pszExtra) \
    HeapAlloc(GetProcessHeap(), dwFlags, cb)

#define DEFAULT_HEAP 0
#define INSTANCE_GLOBAL 0

#define VsDebugInitialize() do {} while (0)
#define VsDebugTerminate() do {} while (0)


// Debug switches
//
#define DEFINE_SWITCH(NAME, PACKAGE, DESC) VSDEFINE_SWITCH(NAME, PACKAGE, DESC)
#define EXTERN_SWITCH(NAME)                VSEXTERN_SWITCH(NAME)
#define FSWITCH(NAME)                      VSFSWITCH(NAME)

#define VSDEFINE_SWITCH(NAME, PACKAGE, DESC)
#define VSEXTERN_SWITCH(NAME)
#define VSFSWITCH(NAME) FALSE

#define VsIgnoreAllocs(f)

#endif // FEATURE_PAL
#endif // __VSASSERT_H__
