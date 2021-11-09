// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

// CS1591 - Missing XML comment for publicly visible type or member 'Type_or_Member' is disabled in this file because it does not ship anymore.
#pragma warning disable 1591

namespace System.Fabric.Testability.Client.Requests
{
    using System;
    using System.Diagnostics.CodeAnalysis;
    using System.Globalization;
    using System.Threading;
    using System.Threading.Tasks;
    using System.Fabric.Description;

    [SuppressMessage("Microsoft.Performance", "CA1812:AvoidUninstantiatedInternalClasses", Justification = "Temporary - for completeness")]
    public class GetSystemServicesListRequest : FabricRequest
    {
        private static readonly Uri SystemApplicationName = new Uri("fabric:/System");

        public GetSystemServicesListRequest(IFabricClient fabricClient, TimeSpan timeout)
            : base(fabricClient, timeout)
        {
        }

        public string ContinuationToken
        {
            get;
            set;
        }

        public override string ToString()
        {
            return string.Format(CultureInfo.InvariantCulture, "GetSystemServicesList with timeout {0}, continuation token \"{1}\"", this.Timeout, this.ContinuationToken);
        }

        public override async Task PerformCoreAsync(CancellationToken cancellationToken)
        {
            this.OperationResult = await this.FabricClient.GetServicePagedListAsync(
                new System.Fabric.Description.ServiceQueryDescription(SystemApplicationName)
                {
                    ContinuationToken = this.ContinuationToken,
                },
                this.Timeout,
                cancellationToken);
        }
    }
}


#pragma warning restore 1591