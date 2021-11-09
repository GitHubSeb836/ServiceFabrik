// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace Common;
using namespace Api;
using namespace Management::ClusterManager;
using namespace ServiceModel;
using namespace Naming;
using namespace HttpGateway;
using namespace Management::FaultAnalysisService;

HttpGateway::RestartPartitionProgress::RestartPartitionProgress()
    : state_(TestCommandProgressState::Invalid)
    , result_()   
{
}

ErrorCode HttpGateway::RestartPartitionProgress::FromInternalInterface(IRestartPartitionProgressResultPtr &resultPtr)
{
    std::shared_ptr<Management::FaultAnalysisService::RestartPartitionProgress> progress = resultPtr->GetProgress();
    state_ = progress->get_State();
    result_ = progress->get_Result();
    return ErrorCode::Success();
}
