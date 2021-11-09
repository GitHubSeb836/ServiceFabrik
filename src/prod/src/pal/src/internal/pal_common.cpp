// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "pal_common.h"

#include "FabricCommon.h"
#include "FabricCommon_.h"

EXTERN_GUID(IID_IFabricAsyncOperationContext, 0x841720bf, 0xc9e8, 0x4e6f, 0x9c, 0x3f, 0x6b, 0x7f, 0x4a, 0xc7, 0x3b, 0xcd);
EXTERN_GUID(IID_IFabricAsyncOperationCallback, 0x86f08d7e, 0x14dd, 0x4575, 0x84, 0x89, 0xb1, 0xd5, 0xd6, 0x79, 0x02, 0x9c);
EXTERN_GUID(IID_IFabricStringResult, 0x4ae69614, 0x7d0f, 0x4cd4, 0xb8, 0x36, 0x23, 0x01, 0x70, 0x00, 0xd1, 0x32);
EXTERN_GUID(IID_IFabricStringListResult, 0xafab1c53, 0x757b, 0x4b0e, 0x8b, 0x7e, 0x23, 0x7a, 0xee, 0xe6, 0xbf, 0xe9);
EXTERN_GUID(IID_IFabricConfigStore2, 0xc8beea34, 0x1f9d, 0x4d3b, 0x97, 0x0d, 0xf2, 0x6c, 0xa0, 0xe4, 0xa7, 0x62);
EXTERN_GUID(IID_IFabricConfigStoreUpdateHandler, 0x792d2f8d, 0x15bd, 0x449f, 0xa6, 0x07, 0x00, 0x2c, 0xb6, 0x00, 0x47, 0x09);

// xmllite.h
EXTERN_GUID(IID_IXmlReader, 0x7279FC81, 0x709D, 0x4095, 0xB6, 0x3D, 0x69, 0xFE, 0x4B, 0x0D, 0x90, 0x30);
EXTERN_GUID(IID_IXmlWriter, 0x7279FC88, 0x709D, 0x4095, 0xB6, 0x3D, 0x69, 0xFE, 0x4B, 0x0D, 0x90, 0x30);
// fabricruntime.h
EXTERN_GUID(IID_IFabricReplicatorSettingsResult, 0x718954f3, 0xdc1e, 0x4060, 0x98, 0x06, 0x0c, 0xbf, 0x36, 0xf7, 0x10, 0x51);
EXTERN_GUID(IID_IFabricBatchOperationStream, 0xf929d72f, 0x9de2, 0x403f, 0xb0, 0x4b, 0xcc, 0x43, 0x24, 0xae, 0x4c, 0x71);
EXTERN_GUID(IID_IFabricOperationStream, 0xa98fb97a, 0xd6b0, 0x408a, 0xa8, 0x78, 0xa9, 0xed, 0xb0, 0x9c, 0x25, 0x87);
EXTERN_GUID(IID_IFabricOperationStream2, 0x0930199b, 0x590a, 0x4065, 0xbe, 0xc9, 0x5f, 0x93, 0xb6, 0xaa, 0xe0, 0x86);
EXTERN_GUID(IID_IFabricInternalReplicator, 0xfe488461, 0x2ed5, 0x4d3b, 0x8e, 0x4e, 0x5d, 0x02, 0xd5, 0x08, 0x27, 0xab);
EXTERN_GUID(IID_IFabricBatchOperation, 0x4ffe9fb9, 0xa70e, 0x4c6a, 0x89, 0x53, 0x8d, 0x2f, 0xf7, 0x9b, 0xcd, 0x9e);
EXTERN_GUID(IID_IFabricPrimaryReplicator, 0x564e50dd, 0xc3a4, 0x4600, 0xa6, 0x0e, 0x66, 0x58, 0x87, 0x43, 0x07, 0xae);
EXTERN_GUID(IID_IFabricInternalStateReplicator, 0x4d76c2bb, 0xf94b, 0x49d0, 0xb0, 0x26, 0xc2, 0x25, 0x91, 0xab, 0x66, 0x77);
EXTERN_GUID(IID_IFabricStateReplicator, 0x89e9a978, 0xc771, 0x44fa, 0x92, 0xe8, 0x3b, 0xf2, 0x71, 0xca, 0xbe, 0x9c);
EXTERN_GUID(IID_IFabricStateReplicator2, 0x4a28d542, 0x658f, 0x46f9, 0x9b, 0xf4, 0x79, 0xb7, 0xca, 0xe2, 0x5c, 0x5d);
EXTERN_GUID(IID_IFabricReplicator, 0x067f144a, 0xe5be, 0xe5be, 0xa1, 0x81, 0x8b, 0x55, 0x93, 0xe2, 0x02, 0x42);
EXTERN_GUID(IID_IFabricOperationData, 0xbab8ad87, 0x37b7, 0x482a, 0x98, 0x5d, 0xba, 0xf3, 0x8a, 0x78, 0x5d, 0xcd);
EXTERN_GUID(IID_IFabricOperationDataStream, 0xc4e9084c, 0xbe92, 0x49c9, 0x8c, 0x18, 0xd4, 0x4d, 0x08, 0x8c, 0x2e, 0x32);
EXTERN_GUID(IID_IFabricStateProvider, 0x3ebfec79, 0xbd27, 0x43f3, 0x8b, 0xe8, 0xda, 0x38, 0xee, 0x72, 0x39, 0x51);
EXTERN_GUID(IID_IFabricStatefulServicePartition, 0x5beccc37, 0x8655, 0x4f20, 0xbd, 0x43, 0xf5, 0x06, 0x91, 0xd7, 0xcd, 0x16);
EXTERN_GUID(IID_IFabricStatefulServicePartition1, 0xc9c66f2f, 0x9dff, 0x4c87, 0xbb, 0xe4, 0xa0, 0x8b, 0x4c, 0x40, 0x74, 0xcf);
EXTERN_GUID(IID_IFabricStatelessServicePartition1, 0xbf6bb505, 0x7bd0, 0x4371, 0xb6, 0xc0, 0xcb, 0xa3, 0x19, 0xa5, 0xe5, 0x0b);

EXTERN_GUID(IID_IFabricConfigStore, 0xedd6091d, 0x5230, 0x4064, 0xa7, 0x31, 0x5d, 0x2e, 0x6b, 0xac, 0x34, 0x36);
EXTERN_GUID(IID_IFabricGetReplicatorStatusResult, 0x30E10C61, 0xA710, 0x4F99, 0xA6, 0x23, 0xBB, 0x14, 0x03, 0x26, 0x51, 0x86);
EXTERN_GUID(IID_IFabricStatefulServiceReplica,    0x8AE3BE0E, 0x505D, 0x4DC1, 0xAD, 0x8F, 0x0C, 0xB0, 0xF9, 0x57, 0x6B, 0x8A);
EXTERN_GUID(IID_IFabricStatefulServiceFactory, 0x77ff0c6b, 0x6780, 0x48ec, 0xb4, 0xb0, 0x61, 0x98, 0x93, 0x27, 0xb0, 0xf2);
EXTERN_GUID(IID_IFabricOperation, 0xf4ad6bfa, 0xe23c, 0x4a48, 0x96, 0x17, 0xc0, 0x99, 0xcd, 0x59, 0xa2, 0x3a);
EXTERN_GUID(IID_IFabricNativeImageStoreClient, 0xf6b3ceea, 0x3577, 0x49fa, 0x8e, 0x67, 0x2d, 0x0a, 0xd1, 0x02, 0x4c, 0x79);
EXTERN_GUID(IID_IFabricStatefulServiceReplicaStatusResult, 0x908dcbb4, 0xbb4e, 0x4b2e, 0xae, 0x0a, 0x46, 0xda, 0xc4, 0x21, 0x84, 0xbc);
EXTERN_GUID(IID_IFabricInternalStatefulServiceReplica, 0xe5395a45, 0xc9fc, 0x43f7, 0xa5, 0xc7, 0x4c, 0x34, 0xd6, 0xb5, 0x67, 0x91);
EXTERN_GUID(IID_IFabricServiceCommunicationClient2, 0x73b2cac5, 0x4278, 0x475b, 0x82, 0xe6, 0x1e, 0x33, 0xeb, 0xe2, 0x07, 0x67);
EXTERN_GUID(IID_IFabricServiceCommunicationClient3, 0xe2430820, 0x95ea, 0x44f7, 0x9c, 0xb9, 0xa4, 0x22, 0x91, 0x89, 0x2f, 0xdc);
EXTERN_GUID(IID_IFabricServiceCommunicationMessage2, 0x15129e8e, 0xaa67, 0x4bf8, 0x94, 0x18, 0x91, 0xd5, 0x87, 0x1f, 0xd3, 0x46);
EXTERN_GUID(IID_IFabricCommunicationMessageHandler2, 0xd969b5b0, 0x7f75, 0x415e, 0xb5, 0x03, 0x55, 0xf8, 0x6b, 0x71, 0xef, 0x96);
EXTERN_GUID(IID_IFabricClientConnection2, 0x716368e5, 0x1aa8, 0x485f, 0xad, 0xeb, 0xa9, 0x49, 0x76, 0x93, 0x04, 0xf5);
