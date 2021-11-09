// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

#include "FabricBackupRestoreService_.h"

BOOL APIENTRY DllMain(
    HMODULE module,
    DWORD reason,
    LPVOID reserved)
{
    UNREFERENCED_PARAMETER(module);
    UNREFERENCED_PARAMETER(reserved);

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}


//
// WARNING: 
//
// The signature below MUST exactly match (including the parameter names)
// with the signature generated by MIDL in the FabricStore.h file. 
// Otherwise the linker does not apply the extern "C" specifier and exports 
// this function as C++ name mangled function instead of C style function.
// 
/* [entry] */ HRESULT CreateFabricBackupRestoreServiceAgent(
    /* [in] */ __RPC__in REFIID riid,
    /* [retval][out] */ __RPC__deref_out_opt void **fabricBackupRestoreServiceAgent)
{
    Common::DllConfig::GetConfig();

    if (Common::TraceTextFileSink::IsEnabled())
    {
        Common::TraceTextFileSink::SetOption(L"m");
    }

    auto factoryPtr = Management::BackupRestoreService::BackupRestoreServiceAgentFactory::Create();
    Api::ComBackupRestoreServiceAgentFactory comFactory(factoryPtr);

    return comFactory.CreateFabricBackupRestoreServiceAgent(riid, fabricBackupRestoreServiceAgent);
}
