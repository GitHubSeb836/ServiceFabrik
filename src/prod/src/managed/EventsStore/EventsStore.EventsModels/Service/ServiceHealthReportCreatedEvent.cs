// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

namespace EventsStore.EventsModels.Service
{
    using System;
    using Newtonsoft.Json;
    using Microsoft.ServiceFabric.Instrumentation.Tracing.Definitions.TypedTraceRecords.HM;

    [JsonObject("ServiceNewHealthReport")]
    public sealed class ServiceHealthReportCreatedEvent : ServiceEvent
    {
        public ServiceHealthReportCreatedEvent(ServiceHealthReportCreatedTraceRecord traceRecord) : base(traceRecord.EventInstanceId, traceRecord.TimeStamp, traceRecord.Category, traceRecord.ServiceName)
        {
            this.InstanceId = traceRecord.InstanceId;
            this.SourceId = traceRecord.SourceId;
            this.Property = traceRecord.Property;
            this.HealthState = traceRecord.HealthState.ToString();
            this.TimeToLiveMs = traceRecord.TTLTimespan;
            this.SequenceNumber = traceRecord.SequenceNumber;
            this.Description = traceRecord.Description;
            this.RemoveWhenExpired = traceRecord.RemoveWhenExpired;
            this.SourceUtcTimestamp = traceRecord.SourceUtcTimestamp;
        }

        [JsonProperty(PropertyName = "InstanceId")]
        public long InstanceId { get; set; }

        [JsonProperty(PropertyName = "SourceId")]
        public string SourceId { get; set; }

        [JsonProperty(PropertyName = "Property")]
        public string Property { get; set; }

        [JsonProperty(PropertyName = "HealthState")]
        public string HealthState { get; set; }

        [JsonProperty(PropertyName = "TimeToLiveMs")]
        public long TimeToLiveMs { get; set; }

        [JsonProperty(PropertyName = "SequenceNumber")]
        public long SequenceNumber { get; set; }

        [JsonProperty(PropertyName = "Description")]
        public string Description { get; set; }

        [JsonProperty(PropertyName = "RemoveWhenExpired")]
        public bool RemoveWhenExpired { get; set; }

        [JsonProperty(PropertyName = "SourceUtcTimestamp")]
        public DateTime SourceUtcTimestamp { get; set; }
    }
}
