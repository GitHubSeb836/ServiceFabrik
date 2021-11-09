// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace Common;
using namespace Federation;
using namespace Transport;
using namespace Reliability;
using namespace Naming;
using namespace std;

using namespace Management::ClusterManager;


AsyncOperationSPtr MoveNextFabricUpgradeDomainAsyncOperation::BeginAcceptRequest(
    ClientRequestSPtr && clientRequest,
    TimeSpan const timeout,
    AsyncCallback const & callback,
    AsyncOperationSPtr const & root)
{
    MoveNextUpgradeDomainMessageBody body;
    if (this->Request.GetBody(body))
    {
        return this->Replica.BeginAcceptMoveNextFabricUpgradeDomain(
            body.NextUpgradeDomain,
            move(clientRequest),
            timeout,
            callback,
            root);
    }
    else
    {
        return this->Replica.RejectInvalidMessage(
            move(clientRequest),
            callback,
            root);
    }
}

ErrorCode MoveNextFabricUpgradeDomainAsyncOperation::EndAcceptRequest(AsyncOperationSPtr const & operation)
{
    ErrorCode error = this->Replica.EndAcceptMoveNextFabricUpgradeDomain(operation);

    // Upgrade context may be enqueued
    if (error.IsError(ErrorCodeValue::CMRequestAlreadyProcessing))
    {
        error = ErrorCodeValue::Success;
    }

    if (error.IsSuccess())
    {
        this->TryComplete(operation->Parent, error);
    }

    return error;
}
