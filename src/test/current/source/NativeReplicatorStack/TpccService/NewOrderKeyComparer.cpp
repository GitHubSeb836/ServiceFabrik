// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace TXRStatefulServiceBase;
using namespace TpccService;
using namespace TxnReplicator;
using namespace Data::Utilities;

static const StringLiteral TraceComponent("NewOrderKeyComparer");

static const ULONG NEWORDER_KEY_CMP_TAG = 'nkct';

NTSTATUS NewOrderKeyComparer::Create(
    __in KAllocator& allocator,
    __out SPtr& result)
{
    NTSTATUS status;
    SPtr output = _new(NEWORDER_KEY_CMP_TAG, allocator) NewOrderKeyComparer();

    if (output == nullptr)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = output->Status();
    if (!NT_SUCCESS(status))
    {
        return status;
    }

    result = Ktl::Move(output);
    return STATUS_SUCCESS;
}

int NewOrderKeyComparer::Compare(
    __in const NewOrderKey::SPtr& lhs,
    __in const NewOrderKey::SPtr& rhs) const
{
    if (lhs == nullptr || rhs == nullptr) {
        Trace.WriteInfo(TraceComponent, "NewOrderKey::SPtr cannot be NULL");
        KInvariant(false);
    }

    if (lhs->Warehouse < rhs->Warehouse)
    {
        return -1;
    }
    else if (lhs->Warehouse > rhs->Warehouse)
    {
        return 1;
    }
    else if (lhs->District < rhs->District)
    {
        return -1;
    }
    else if (lhs->District > rhs->District)
    {
        return 1;
    }
    else if (lhs->Order < rhs->Order)
    {
        return -1;
    }
    else if (lhs->Order > rhs->Order)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

NewOrderKeyComparer::NewOrderKeyComparer()
{
}

NewOrderKeyComparer::~NewOrderKeyComparer()
{
}
