// ----------------------------------------------------------------------
//  <copyright file="StartNetwork.cs" company="Microsoft">
//       Copyright (c) Microsoft Corporation. All rights reserved.
//  </copyright>
// ----------------------------------------------------------------------

namespace Microsoft.ServiceFabric.Powershell
{
    using System;
    using System.Management.Automation;

    [Cmdlet(VerbsLifecycle.Start, "ServiceFabricNetwork")]
    public sealed class StartNetwork : CommonCmdletBase
    {
        [Parameter(Mandatory = true)]
        public Uri NetworkName
        {
            get;
            set;
        }

        protected override void ProcessRecord()
        {
            this.WriteVerbose("Implementation to be added for \"StartNetwork\" function");
        }
    }
}
