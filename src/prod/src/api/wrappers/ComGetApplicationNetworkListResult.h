// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

namespace Api
{
    // This GUID should be the same for all list results
    // {5CAE4128-B670-4F09-905B-3B6CAFA36E92}
    static const GUID CLSID_ComGetApplicationNetworkListResult =
    {0x5cae4128,0xb670,0x4f09,{0x90,0x5b,0x3b,0x6c,0xaf,0xa3,0x6e,0x92}};
    
    class ComGetApplicationNetworkListResult :
        public IFabricGetApplicationNetworkListResult,
        private Common::ComUnknownBase
    {
        DENY_COPY_ASSIGNMENT(ComGetApplicationNetworkListResult)

        COM_INTERFACE_LIST2(
            ComGetApplicationNetworkListResult,
            IID_IFabricGetApplicationNetworkListResult,
            IFabricGetApplicationNetworkListResult,
            CLSID_ComGetApplicationNetworkListResult,
            ComGetApplicationNetworkListResult)

    public:
        ComGetApplicationNetworkListResult(
            std::vector<ServiceModel::ApplicationNetworkQueryResult> && applicationNetworkList,
            ServiceModel::PagingStatusUPtr && pagingStatus);

        // 
        // IFabricGetApplicationNetworkListResult methods
        // 
        const FABRIC_APPLICATION_NETWORK_QUERY_RESULT_LIST *STDMETHODCALLTYPE get_ApplicationNetworkList( void );
        const FABRIC_PAGING_STATUS *STDMETHODCALLTYPE get_PagingStatus( void );

    private:
        Common::ReferencePointer<FABRIC_APPLICATION_NETWORK_QUERY_RESULT_LIST> applicationNetworkList_;
        Common::ReferencePointer<FABRIC_PAGING_STATUS> pagingStatus_;
        Common::ScopedHeap heap_;
    };

    typedef Common::ComPointer<ComGetApplicationNetworkListResult> ComGetApplicationNetworkListResultCPtr;
}