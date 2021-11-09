// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace std;
using namespace Common;
using namespace ServiceModel;
using namespace ServiceModel::QueryResultHelpers;

BEGIN_QUERY_RESULT_ITEM_ENUM()    
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::Node)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::Service)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceGroupMember)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationType)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::Application)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServicePartition)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceReplica)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::QueryMetadata)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::String)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceType)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceGroupMemberType)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedApplication)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServiceManifest)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServiceReplica)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServiceType)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedCodePackage)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationQueryResult)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::NodeHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ReplicaHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::PartitionHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedApplicationHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServicePackageHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ClusterHealth)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::InfrastructureTask)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::NodeAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ReplicaAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::PartitionAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedApplicationAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServicePackageAggregatedHealthState)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::DeployedServiceReplicaDetail)
    QUERY_RESULT_ITEM_ENUM_KIND_WITH_FACTORY(QueryResultHelpers::RepairTask, RepairTask)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ClusterLoadInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::PartitionLoadInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ProvisionedFabricCodeVersion)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ProvisionedFabricConfigVersion)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::InternalDeletedApplicationsQueryObject)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::InternalProvisionedApplicationTypeQueryResult)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::PackageSharingPolicyQueryObject)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::PackageSharingPolicyList)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::NodeLoadInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ReplicaLoadInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::UnplacedReplicaInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationLoadInformation)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ClusterHealthChunk)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::TestCommandList)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ServiceName)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ApplicationName)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ComposeDeploymentStatus)
    QUERY_RESULT_ITEM_ENUM_KIND(QueryResultHelpers::ComposeDeploymentUpgradeProgress)
END_QUERY_RESULT_ITEM_ENUM()

