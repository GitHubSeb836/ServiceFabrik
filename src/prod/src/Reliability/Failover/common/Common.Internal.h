// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

#include "Reliability/LoadBalancing/common/Public.h"

// Messages
#include "Reliability/Failover/common/Constants.h"
#include "Reliability/Failover/common/ServiceTypeInfo.h"
#include "Reliability/Failover/common/ServiceTypeNotificationRequestMessageBody.h"
#include "Reliability/Failover/common/ServiceTypeNotificationReplyMessageBody.h"

#include "Reliability/Failover/common/FailoverManagerId.h"
#include "Reliability/Failover/common/GenerationHeader.h"
#include "Reliability/Failover/common/FailoverUnitDescription.h"
#include "Reliability/Failover/common/FailoverUnitMessageBody.h"
#include "Reliability/Failover/common/ReplicaRole.h"
#include "Reliability/Failover/common/ReplicaRoleHelper.h"
#include "Reliability/Failover/common/ReplicaStatus.h"
#include "Reliability/Failover/common/ReplicaStates.h"
#include "Reliability/Failover/common/PlbMovementIgnoredReasons.h"
#include "Reliability/Failover/common/ReplicaDescription.h"
#include "Reliability/Failover/common/FailoverUnitDescription.h"
#include "Reliability/Failover/common/ReplicaInfo.h"
#include "Reliability/Failover/common/FailoverUnitInfo.h"
#include "Reliability/Failover/common/ReplicaDeactivationInfo.h"
#include "Reliability/Failover/common/FailoverUnitMessageBody.h"
#include "Reliability/Failover/common/ConfigurationMessageBody.h"
#include "Reliability/Failover/common/DoReconfigurationMessageBody.h"
#include "Reliability/Failover/common/DeactivateMessageBody.h"
#include "Reliability/Failover/common/ActivateMessageBody.h"
#include "Reliability/Failover/common/ReplicaListMessageBody.h"
#include "Reliability/Failover/common/ReplicaUpMessageBody.h"
#include "Reliability/Failover/common/NodeFabricUpgradeReplyMessageBody.h"
#include "Reliability/Failover/common/NodeUpgradeReplyMessageBody.h"
#include "Reliability/Failover/common/NodeActivateRequestMessageBody.h"
#include "Reliability/Failover/common/NodeDeactivateRequestMessageBody.h"
#include "Reliability/Failover/common/NodeActivateReplyMessageBody.h"
#include "Reliability/Failover/common/NodeDeactivateReplyMessageBody.h"
#include "Reliability/Failover/common/NodeDescription.h"
#include "Reliability/Failover/common/NodeUpMessageBody.h"
#include "Reliability/Failover/common/NodeUpAckMessageBody.h"
#include "Reliability/Failover/common/ChangeNotificationChallenge.h"
#include "Reliability/Failover/common/SendReceiveToFMOperation.h"
#include "Reliability/Failover/common/LFUMMessageBody.h"
#include "Reliability/Failover/common/NodeUpOperationFactory.h"
#include "Reliability/Failover/common/NodeUpOperation.h"
#include "Reliability/Failover/common/RALFUMUploadOperation.h"
#include "Reliability/Failover/common/ReplicaMessageBody.h"
#include "Reliability/Failover/common/DeleteReplicaMessageBody.h"
#include "Reliability/Failover/common/RAReplicaMessageBody.h"
#include "Reliability/Failover/common/GenerationProposalReplyMessageBody.h"
#include "Reliability/Failover/common/GenerationUpdateMessageBody.h"
#include "Reliability/Failover/common/NodeUpdateServiceRequestMessageBody.h"
#include "Reliability/Failover/common/NodeUpdateServiceReplyMessageBody.h"
#include "Reliability/Failover/common/VersionedCuid.h"
#include "Reliability/Failover/common/PersistenceState.h"
#include "Reliability/Failover/common/FailoverUnitReplyMessageBody.h"
#include "Reliability/Failover/common/ReplicaReplyMessageBody.h"
#include "Reliability/Failover/common/GetLSNReplyMessageBody.h"
#include "Reliability/Failover/common/ConfigurationReplyMessageBody.h"
#include "Reliability/Failover/common/FailoverUnitMessageBody.h"
#include "Reliability/Failover/common/ReliabilityEventSource.h"
#include "Reliability/Failover/common/ReportLoadInfo.h"
#include "Reliability/Failover/common/ReportLoadMessageBody.h"
#include "Reliability/Failover/common/UpgradeDescription.h"
#include "Reliability/Failover/common/RSMessage.h"
#include "Reliability/Failover/common/NodeDescription.h"
#include "Reliability/Failover/common/FailoverConfig.h"
#include "Reliability/Failover/common/ServiceTypeUpdateMessageBody.h"
#include "Reliability/Failover/common/ServiceTypeUpdateReplyMessageBody.h"
#include "Reliability/Failover/common/PLBLoadUtility.h"
#include "Reliability/Failover/common/ReplicaFlags.h"
#include "Reliability/Failover/common/FailoverUnitFlags.h"
#include "Reliability/Failover/common/PartitionNotificationMessageBody.h"
#include "Reliability/Failover/common/AvailableContainerImagesMessageBody.h"

#include "Management/NetworkInventoryManager/common/NetworkInventoryManagerConfig.h"
#include "Management/NetworkInventoryManager/common/NIMMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkErrorCodeResponseMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkDefinition.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkAllocationEntry.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkAllocationRequestMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkAllocationResponseMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkCreateRequestMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkCreateResponseMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkRemoveRequestMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkEnumerateRequestMessage.h"
#include "Management/NetworkInventoryManager/common/NIMNetworkEnumerateResponseMessage.h"
#include "Management/NetworkInventoryManager/common/NIMPublishNetworkTablesRequestMessageBody.h"
#include "Management/NetworkInventoryManager/common/NIMPublishNetworkTablesRequestMessage.h"


//Common job item root interface
#include "Reliability/Failover/common/IMessageMetadata.h"
#include "Reliability/Failover/common/IMessageHandler.h"
#include "Reliability/Failover/common/MessageProcessingJobItem.h"
