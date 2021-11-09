// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

#include "Common/Common.h"
#include "ServiceModel/ServiceModel.h"
#include "Transport/Transport.h"
#include "Federation/Federation.h"
#include "systemservices/SystemServices.Messages.h"
#include "Naming/INamingMessageProcessor.h"
#include "Naming/ClientIdentityHeader.h"
#include "Management/RepairManager/RepairManager.External.h"

// Message body between client and server

#include "client/ClientServerTransport/DeactivateNodesRequestMessageBody.h"
#include "client/ClientServerTransport/NodeDeactivationStatusReplyMessageBody.h"
#include "client/ClientServerTransport/NodeDeactivationStatusRequestMessageBody.h"
#include "client/ClientServerTransport/RemoveNodeDeactivationRequestMessageBody.h"
#include "client/ClientServerTransport/ReportFaultReplymessageBody.h"
#include "client/ClientServerTransport/ReportFaultRequestMessageBody.h"
#include "client/ClientServerTransport/ActivateNodeMessageBody.h"
#include "client/ClientServerTransport/ClientOperationFailureMessageBody.h"
#include "client/ClientServerTransport/CreateSystemServiceMessageBody.h"
#include "client/ClientServerTransport/DeactivateNodeMessageBody.h"
#include "client/ClientServerTransport/DeleteServiceMessageBody.h"
#include "client/ClientServerTransport/DeleteSystemServiceMessageBody.h"
#include "client/ClientServerTransport/FileDownloadMessageBody.h"
#include "client/ClientServerTransport/NameExistsReplyMessageBody.h"
#include "client/ClientServerTransport/NodeStateRemovedMessageBody.h"
#include "client/ClientServerTransport/RecoverPartitionMessageBody.h"
#include "client/ClientServerTransport/RecoverPartitionsMessageBody.h"
#include "client/ClientServerTransport/RecoverServicePartitionsMessageBody.h"
#include "client/ClientServerTransport/RecoverSystemPartitionsMessageBody.h"
#include "client/ClientServerTransport/ServiceResolutionMessageBody.h"
#include "client/ClientServerTransport/StartNodeRequestMessageBody.h"
#include "client/ClientServerTransport/NotificationClientConnectionReplyBody.h"
#include "client/ClientServerTransport/NotificationClientConnectionRequestBody.h"
#include "client/ClientServerTransport/NotificationClientSynchronizationReplyBody.h"
#include "client/ClientServerTransport/NotificationClientSynchronizationRequestBody.h"
#include "client/ClientServerTransport/RegisterServiceNotificationFilterRequestBody.h"
#include "client/ClientServerTransport/ResolveSystemServiceReplybody.h"
#include "client/ClientServerTransport/ResolveSystemServiceRequestBody.h"
#include "client/ClientServerTransport/ServiceLocationNotificationReplyBody.h"
#include "client/ClientServerTransport/ServiceLocationNotificationRequestBody.h"
#include "client/ClientServerTransport/ServiceNotificationReplyBody.h"
#include "client/ClientServerTransport/ServiceNotificationRequestBody.h"
#include "client/ClientServerTransport/UnregisterServiceNotificationFilterRequestBody.h"
#include "client/ClientServerTransport/UpdateServiceRequestBody.h"
#include "client/ClientServerTransport/VersionedReplyBody.h"
#include "client/ClientServerTransport/CreateApplicationMessageBody.h"
#include "client/ClientServerTransport/UpdateApplicationMessageBody.h"
#include "client/ClientServerTransport/CreateServiceFromTemplateMessageBody.h"
#include "client/ClientServerTransport/InfrastructureTaskReplyBody.h"
#include "client/ClientServerTransport/MoveNextUpgradeDomainMessageBody.h"
#include "client/ClientServerTransport/ProvisionFabricBody.h"
#include "client/ClientServerTransport/RollbackApplicationUpgradeMessageBody.h"
#include "client/ClientServerTransport/UpdateApplicationUpgradeMessageBody.h"
#include "client/ClientServerTransport/UpdateFabricUpgradeMessageBody.h"
#include "client/ClientServerTransport/ListRequest.h"
#include "client/ClientServerTransport/ListReply.h"
#include "client/ClientServerTransport/UploadRequest.h"
#include "client/ClientServerTransport/ImageStoreBaseRequest.h"
#include "client/ClientServerTransport/CopyRequest.h"
#include "client/ClientServerTransport/InternalListReply.h"
#include "client/ClientServerTransport/ShareLocationReply.h"
#include "client/ClientServerTransport/ResetPartitionLoadMessageBody.h"
#include "client/ClientServerTransport/ToggleVerboseServicePlacementHealthReportingMessageBody.h"
#include "client/ClientServerTransport/uploadsessionrequest.h"
#include "client/ClientServerTransport/uploadchunkrequest.h"
#include "client/ClientServerTransport/UploadChunkContentRequest.h"
#include "client/ClientServerTransport/deleteuploadsessionrequest.h"
#include "client/ClientServerTransport/createuploadsessionrequest.h"
#include "client/ClientServerTransport/ImageStoreContentExistsReplyMessageBody.h"
#include "client/ClientServerTransport/DeleteComposeDeploymentMessageBody.h"
#include "client/ClientServerTransport/DeleteSingleInstanceDeploymentMessageBody.h"
#include "client/ClientServerTransport/RollbackComposeDeploymentMessageBody.h"
#include "client/ClientServerTransport/NIMCreateNetworkMessageBody.h"
#include "client/ClientServerTransport/NIMDeleteNetworkMessageBody.h"
#include "client/ClientServerTransport/NIMValidateNetworkMessageBody.h"

//
// Internal ClientServer Transport public headers
//
#include "client/ClientServerTransport/SystemServiceTcpMessageBase.h"
#include "client/ClientServerTransport/ClientServerRequestMessage.h"
#include "client/ClientServerTransport/ClientServerRequestMessage_TransportMessageWrapper.h"
#include "client/ClientServerTransport/ClientServerReplyMessage.h"
#include "client/ClientServerTransport/IClientServerTransport.h"
#include "client/ClientServerTransport/NamingUriMessageBody.h"
#include "client/ClientServerTransport/DeleteApplicationMessageBody.h"
#include "client/ClientServerTransport/FabricVersionMessageBody.h"
#include "client/ClientServerTransport/CreateServiceMessageBody.h"
#include "client/ClientServerTransport/CreateApplicationResourceMessageBody.h"
#include "client/ClientServerTransport/ClusterManagerTcpMessage.h"

#include "client/ClientServerTransport/ResourceManagerMessage.h"

#include "client/ClientServerTransport/CreateComposeDeploymentRequestHeader.h"
#include "client/ClientServerTransport/UpgradeComposeDeploymentRequestHeader.h"
#include "client/ClientServerTransport/ContainerOperationTcpMessage.h"

#include "client/ClientServerTransport/CreateVolumeMessageBody.h"
#include "client/ClientServerTransport/DeleteVolumeMessageBody.h"
#include "client/ClientServerTransport/VolumeOperationTcpMessage.h"

#include "client/ClientServerTransport/QueryAddressHeader.h"
#include "client/ClientServerTransport/QueryTcpMessage.h"
#include "client/ClientServerTransport/FileSequenceHeader.h"
#include "client/ClientServerTransport/FileUploadRequestHeader.h"
#include "client/ClientServerTransport/FileUploadCreateRequestHeader.h"

#include "client/ClientServerTransport/TokenValidationServiceTcpMessage.h"
#include "client/ClientServerTransport/InfrastructureServiceTcpMessage.h"
#include "client/ClientServerTransport/FaultAnalysisServiceTcpMessage.h"
#include "client/ClientServerTransport/FileStoreServiceTcpMessage.h"
#include "client/ClientServerTransport/HealthManagerTcpMessage.h"
#include "client/ClientServerTransport/RepairManagerTcpMessage.h"
#include "client/ClientServerTransport/UpgradeOrchestrationServiceTcpMessage.h"
#include "client/ClientServerTransport/CentralSecretServiceMessage.h"
#include "client/ClientServerTransport/GatewayResourceManagerTcpMessage.h"

#include "client/ClientServerTransport/ClientProtocolVersionHeader.h"
#include "client/ClientServerTransport/PingReplyMessageBody.h"
#include "client/ClientServerTransport/NamingTcpMessage.h"
#include "client/ClientServerTransport/FileTransferTcpMessage.h"
#include "client/ClientServerTransport/DeleteNameHeader.h"
#include "client/ClientServerTransport/PartitionTargetHeader.h"
#include "client/ClientServerTransport/ServiceTargetHeader.h"
#include "client/ClientServerTransport/SecondaryLocationsHeader.h"
#include "client/ClientServerTransport/NamingPropertyHeader.h"
#include "client/ClientServerTransport/ClientServerTcpTransport.h"
#include "client/ClientServerTransport/ClientServerPassThroughTransport.h"
#include "client/ClientServerTransport/PassThroughReceiverContext.h"
#include "client/ClientServerTransport/PassThroughSendTarget.h"