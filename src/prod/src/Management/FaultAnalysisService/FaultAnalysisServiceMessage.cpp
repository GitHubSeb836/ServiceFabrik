// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace Transport;
using namespace ServiceModel;
using namespace SystemServices;
using namespace Management::FaultAnalysisService;

GlobalWString FaultAnalysisServiceMessage::OperationSuccessAction = make_global<wstring>(L"OperationSuccessAction");

void FaultAnalysisServiceMessage::WrapForFaultAnalysisService(__inout Message & message)
{
    ServiceRoutingAgentMessage::WrapForRoutingAgent(
        message,
        ServiceTypeIdentifier(
            ServicePackageIdentifier(
                ApplicationIdentifier::FabricSystemAppId->ToString(),
                SystemServiceApplicationNameHelper::FaultAnalysisServicePackageName),
            SystemServiceApplicationNameHelper::FaultAnalysisServiceType));
}

void FaultAnalysisServiceMessage::SetHeaders(Message & message, wstring const & action)
{
    message.Headers.Add(ActionHeader(action));
    message.Headers.Add(ActorHeader(Actor::FAS));
    message.Headers.Add(FabricActivityHeader(Guid::NewGuid()));
}
