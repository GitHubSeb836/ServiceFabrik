// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#include "stdafx.h"

using namespace Common;
using namespace Data;
using namespace Data::TestCommon;

namespace Data
{
    ComAsyncOperationCallbackTestHelper::ComAsyncOperationCallbackTestHelper(std::function<void(IFabricAsyncOperationContext *)> const & callback)
        : callback_(callback)
    {
    }

    void STDMETHODCALLTYPE ComAsyncOperationCallbackTestHelper::Invoke(IFabricAsyncOperationContext * aoc)
    {
        if (callback_)
        {
            callback_(aoc);
        }
    }
}
