// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

#pragma once
//+
// Provider Microsoft-Windows-CEIP Event Count 7
//+
EXTERN_C __declspec(selectany) const GUID S_Microsoft_Windows_CEIP = {0xa402fe09, 0xda6e, 0x45f2, {0x82, 0xaf, 0x3c, 0xb3, 0x71, 0x70, 0xee, 0x0c}};

//
// Event Descriptors
//
#define WSQMCONS_CONSOLIDATION_SUCCESS 0x400003EDL
#define WSQMCONS_CONSOLIDATION_ERROR 0xC00003EEL
#define WSQMCONS_UPLOAD_SUCCESS 0x400003EFL
#define WSQMCONS_UPLOAD_ERROR 0xC00003F0L
#define WSQMCONS_NOTIFY_SUCCESS 0x400003F1L
#define WSQMCONS_NOTIFY_ERROR 0xC00003F2L
#define WSQMCONS_MANIFEST_DOWNLOAD 0xC00003F3L
//+
// Provider Microsoft-Windows-SQM-Events Event Count 26
//+
EXTERN_C __declspec(selectany) const GUID Microsoft_Windows_SQM_Provider = {0xa97524f6, 0x064c, 0x4c4e, {0xb7, 0x4b, 0x1a, 0xcc, 0x87, 0xc3, 0x70, 0x0d}};

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR Microsoft_Windows_SQM_Provider_EVENT_0x3_0_0_4_0_0_8000000000000 = {0x3, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define Microsoft_Windows_SQM_Provider_EVENT_0x3_0_0_4_0_0_8000000000000_value 0x3
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR Microsoft_Windows_SQM_Provider_EVENT_0x4_0_0_4_0_0_8000000000000 = {0x4, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define Microsoft_Windows_SQM_Provider_EVENT_0x4_0_0_4_0_0_8000000000000_value 0x4
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SET_DWORD_V0 = {0x5, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SET_DWORD_V0_value 0x5
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_INCREMENT_DWORD_V0 = {0x6, 0x0, 0x0, 0x4, 0x2, 0x0, 0x8000000000000};
#define SQM_INCREMENT_DWORD_V0_value 0x6
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_ADDTOAVERAGE_DWORD_V0 = {0x7, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_ADDTOAVERAGE_DWORD_V0_value 0x7
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SETIFMAX_DWORD_V0 = {0x8, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SETIFMAX_DWORD_V0_value 0x8
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SETIFMIN_DWORD_V0 = {0x9, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SETIFMIN_DWORD_V0_value 0x9
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR Microsoft_Windows_SQM_Provider_EVENT_0xb_0_0_4_0_0_8000000000000 = {0xb, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define Microsoft_Windows_SQM_Provider_EVENT_0xb_0_0_4_0_0_8000000000000_value 0xb
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_INIT = {0x1, 0x0, 0x0, 0x4, 0x1, 0x0, 0x8000000000000};
#define SQM_INIT_value 0x1
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_CLEANUP = {0x2, 0x0, 0x0, 0x4, 0x2, 0x0, 0x8000000000000};
#define SQM_CLEANUP_value 0x2
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_START_SESSION = {0x3, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_START_SESSION_value 0x3
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_END_SESSION = {0x4, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_END_SESSION_value 0x4
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SET_DWORD = {0x5, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SET_DWORD_value 0x5
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_INCREMENT_DWORD = {0x6, 0x1, 0x0, 0x4, 0x2, 0x0, 0x8000000000000};
#define SQM_INCREMENT_DWORD_value 0x6
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_ADDTOAVERAGE_DWORD = {0x7, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_ADDTOAVERAGE_DWORD_value 0x7
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SETIFMAX_DWORD = {0x8, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SETIFMAX_DWORD_value 0x8
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SETIFMIN_DWORD = {0x9, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SETIFMIN_DWORD_value 0x9
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SET_STRING = {0xa, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SET_STRING_value 0xa
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_ADD_LEGACYSTREAMROW = {0xb, 0x1, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_ADD_LEGACYSTREAMROW_value 0xb
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_LOGON_USER = {0xc, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_LOGON_USER_value 0xc
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_LOGOFF_USER = {0xd, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_LOGOFF_USER_value 0xd
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_OPTIN_NOTIFICATION_SHOWN = {0xe, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_OPTIN_NOTIFICATION_SHOWN_value 0xe
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_OPTIN_NOTIFICATION_RESPONSE = {0xf, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_OPTIN_NOTIFICATION_RESPONSE_value 0xf
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_SET_DWORD64 = {0x10, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_SET_DWORD64_value 0x10
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR SQM_ADD_STREAMROW = {0x11, 0x0, 0x0, 0x4, 0x0, 0x0, 0x8000000000000};
#define SQM_ADD_STREAMROW_value 0x11
#define MSG_event_CEIP_EVENT_SOURCE          0x90000001L
