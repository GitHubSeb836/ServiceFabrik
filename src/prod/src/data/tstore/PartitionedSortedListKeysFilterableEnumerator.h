// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once

namespace Data
{
    namespace TStore
    {
        template<typename TKey, typename TValue>
        class PartitionedSortedListKeysFilterableEnumerator : public IFilterableEnumerator<TKey>
        {
            K_FORCE_SHARED(PartitionedSortedListKeysFilterableEnumerator)

        public:
            static NTSTATUS Create(
                __in PartitionedSortedListFilterableEnumerator<TKey, TValue> & keyValueEnumerator,
                __in KAllocator & allocator,
                __out SPtr & result)
            {
                NTSTATUS status;
                SPtr output = _new(PENUM_TAG, allocator) PartitionedSortedListKeysFilterableEnumerator(keyValueEnumerator);

                if (!output)
                {
                    status = STATUS_INSUFFICIENT_RESOURCES;
                    return status;
                }

                status = output->Status();
                if (!NT_SUCCESS(status))
                {
                    return status;
                }

                result = Ktl::Move(output);
                return STATUS_SUCCESS;
            }

            TKey Current() override
            {
                KeyValuePair<TKey, TValue> current = keyValueEnumeratorSPtr_->Current();
                return current.Key;
            }

            bool MoveNext()
            {
                return keyValueEnumeratorSPtr_->MoveNext();
            }

            bool MoveTo(TKey const & key)
            {
                return keyValueEnumeratorSPtr_->MoveTo(KeyValuePair<TKey, TValue>(key, TValue()));
            }

        private:
            PartitionedSortedListKeysFilterableEnumerator(__in PartitionedSortedListFilterableEnumerator<TKey, TValue> & keyValueEnumerator);
            KSharedPtr<PartitionedSortedListFilterableEnumerator<TKey, TValue>> keyValueEnumeratorSPtr_;
        };
        
        template<typename TKey, typename TValue>
        PartitionedSortedListKeysFilterableEnumerator<TKey, TValue>::PartitionedSortedListKeysFilterableEnumerator(__in PartitionedSortedListFilterableEnumerator<TKey, TValue> & keyValueEnumerator)
            : keyValueEnumeratorSPtr_(&keyValueEnumerator)
        {
        }

        template<typename TKey, typename TValue>
        PartitionedSortedListKeysFilterableEnumerator<TKey, TValue>::~PartitionedSortedListKeysFilterableEnumerator()
        {
        }
    }
}
