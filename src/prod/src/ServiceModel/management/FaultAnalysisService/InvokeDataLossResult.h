// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

namespace Management
{
    namespace FaultAnalysisService
    {
        class InvokeDataLossResult : public ServiceModel::ClientServerMessageBody
        {
            DEFAULT_COPY_CONSTRUCTOR(InvokeDataLossResult)

        public:
            InvokeDataLossResult();

            InvokeDataLossResult(InvokeDataLossResult &&);

            __declspec(property(get = get_ErrorCode)) HRESULT ErrorCode;
            HRESULT const & get_ErrorCode() const { return errorCode_; }

            __declspec(property(get = get_SelectedPartition)) std::shared_ptr<Management::FaultAnalysisService::SelectedPartition> const& Partition;
            std::shared_ptr<Management::FaultAnalysisService::SelectedPartition> const& get_SelectedPartition() const { return selectedPartition_; }
            
            Common::ErrorCode FromPublicApi(FABRIC_PARTITION_DATA_LOSS_RESULT const &);
            void ToPublicApi(__in Common::ScopedHeap &, __out FABRIC_PARTITION_DATA_LOSS_RESULT &) const;

            FABRIC_FIELDS_02(errorCode_, selectedPartition_);

            BEGIN_JSON_SERIALIZABLE_PROPERTIES()
                SERIALIZABLE_PROPERTY(ServiceModel::Constants::ErrorCode, errorCode_)
                SERIALIZABLE_PROPERTY(ServiceModel::Constants::SelectedPartition, selectedPartition_)
            END_JSON_SERIALIZABLE_PROPERTIES()

        private:
            HRESULT errorCode_;
            std::shared_ptr<Management::FaultAnalysisService::SelectedPartition> selectedPartition_;
        };
    }
}
