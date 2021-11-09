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
using namespace ClientServerTransport;

using namespace Management::RepairManager;

StringLiteral const TraceComponent("CreateRepairRequestAsyncOperation");

CreateRepairRequestAsyncOperation::CreateRepairRequestAsyncOperation(
    __in RepairManagerServiceReplica & owner,
    MessageUPtr && request,
    IpcReceiverContextUPtr && receiverContext,
    TimeSpan const timeout,
    AsyncCallback const & callback,
    AsyncOperationSPtr const & root)
    : ClientRequestAsyncOperation(
        owner,
        move(request),
        move(receiverContext),
        timeout,
        callback,
        root)
{
}

AsyncOperationSPtr CreateRepairRequestAsyncOperation::BeginAcceptRequest(
    ClientRequestSPtr && clientRequest,
    TimeSpan const timeout,
    AsyncCallback const & callback,
    AsyncOperationSPtr const & root)
{
    UpdateRepairTaskMessageBody body;
    if (this->Request.GetBody(body))
    {
        return this->Replica.BeginAcceptCreateRepairRequest(
            move(body.MutableTask),
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

ErrorCode CreateRepairRequestAsyncOperation::EndAcceptRequest(AsyncOperationSPtr const & operation)
{
    int64 commitVersion = 0;
    ErrorCode error = this->Replica.EndAcceptCreateRepairRequest(operation, commitVersion);

    if (error.IsSuccess())
    {
        auto body = Common::make_unique<UpdateRepairTaskReplyMessageBody>(commitVersion);
        this->SetReply(RepairManagerTcpMessage::GetClientOperationSuccess(std::move(body))->GetTcpMessage());
    }

    return error;
}
