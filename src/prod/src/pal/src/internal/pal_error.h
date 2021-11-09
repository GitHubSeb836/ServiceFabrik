// ------------------------------------------------------------
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
// ------------------------------------------------------------

/*++





--*/

#ifndef __PAL_ERROR_H__
#define __PAL_ERROR_H__

/******************* error codes *****************************************/
/* STARTERRORCODES - do not remove this marker, it is consumed by the palsatellite tool */

#define ERROR_SUCCESS 0L
#define ERROR_INVALID_FUNCTION 1L
#define ERROR_FILE_NOT_FOUND 2L
#define ERROR_PATH_NOT_FOUND 3L
#define ERROR_TOO_MANY_OPEN_FILES 4L
#define ERROR_ACCESS_DENIED 5L
#define ERROR_INVALID_HANDLE 6L
#define ERROR_NOT_ENOUGH_MEMORY 8L
#define ERROR_BAD_ENVIRONMENT 10L
#define ERROR_BAD_FORMAT 11L
#define ERROR_INVALID_ACCESS 12L
#define ERROR_INVALID_DATA 13L
#define ERROR_OUTOFMEMORY 14L
#define ERROR_INVALID_DRIVE 15L
#define ERROR_NO_MORE_FILES 18L
#define ERROR_WRITE_PROTECT 19L
#define ERROR_NOT_READY 21L
#define ERROR_BAD_COMMAND 22L

#define ERROR_BAD_LENGTH 24L
#define ERROR_WRITE_FAULT 29L
#define ERROR_READ_FAULT 30L
#define ERROR_GEN_FAILURE 31L
#define ERROR_SHARING_VIOLATION 32L
#define ERROR_LOCK_VIOLATION 33L
#define ERROR_SHARING_BUFFER_EXCEEDED 36L
#define ERROR_HANDLE_EOF 38L
#define ERROR_HANDLE_DISK_FULL 39L
#define ERROR_NOT_SUPPORTED 50L
#define ERROR_DUP_NAME 52L
#define ERROR_BAD_NETPATH 53L
#define ERROR_DEV_NOT_EXIST 55L
#define ERROR_BAD_NET_NAME 67L
#define ERROR_FILE_EXISTS 80L
#define ERROR_CANNOT_MAKE 82L
#define ERROR_INVALID_PARAMETER 87L
#define ERROR_NET_WRITE_FAULT 88L
#define ERROR_TOO_MANY_SEMAPHORES 100L
#define ERROR_DRIVE_LOCKED 108L
#define ERROR_BROKEN_PIPE 109L
#define ERROR_OPEN_FAILED 110L
#define ERROR_BUFFER_OVERFLOW 111L
#define ERROR_DISK_FULL 112L
#define ERROR_CALL_NOT_IMPLEMENTED 120L
#define ERROR_SEM_TIMEOUT 121L
#define ERROR_INSUFFICIENT_BUFFER 122L
#define ERROR_INVALID_NAME 123L
#define ERROR_MOD_NOT_FOUND 126L
#define ERROR_PROC_NOT_FOUND 127L
#define ERROR_WAIT_NO_CHILDREN 128L
#define ERROR_NEGATIVE_SEEK 131L
#define ERROR_SEEK_ON_DEVICE 132L
#define ERROR_DIR_NOT_EMPTY 145L
#define ERROR_SIGNAL_REFUSED 156L
#define ERROR_NOT_LOCKED 158L
#define ERROR_BAD_PATHNAME 161L
#define ERROR_BUSY 170L

#define ERROR_INVALID_ORDINAL 182L
#define ERROR_ALREADY_EXISTS 183L
#define ERROR_INVALID_EXE_SIGNATURE 191L
#define ERROR_EXE_MARKED_INVALID 192L
#define ERROR_BAD_EXE_FORMAT 193L
#define ERROR_ENVVAR_NOT_FOUND 203L
#define ERROR_FILENAME_EXCED_RANGE 206L
#define ERROR_PIPE_BUSY 231L
#define ERROR_NO_DATA 232L
#define ERROR_MORE_DATA 234L
#define ERROR_NO_MORE_ITEMS 259L
#define ERROR_DIRECTORY 267L
#define ERROR_NOT_OWNER 288L
#define ERROR_TOO_MANY_POSTS 298L
#define ERROR_PARTIAL_COPY 299L
#define ERROR_INVALID_ADDRESS 487L
#define ERROR_ARITHMETIC_OVERFLOW 534L
#define ERROR_OPERATION_ABORTED 995L
#define ERROR_IO_INCOMPLETE 996L
#define ERROR_IO_PENDING 997L
#define ERROR_NOACCESS 998L
#define ERROR_STACK_OVERFLOW 1001L
#define ERROR_INVALID_FLAGS 1004L
#define ERROR_UNRECOGNIZED_VOLUME 1005L
#define ERROR_FILE_INVALID 1006L
#define ERROR_PROCESS_ABORTED 1067L
#define ERROR_NO_UNICODE_TRANSLATION 1113L
#define ERROR_DLL_INIT_FAILED 1114L
#define ERROR_IO_DEVICE 1117L
#define ERROR_DISK_OPERATION_FAILED 1127L
#define ERROR_POSSIBLE_DEADLOCK 1131L
#define ERROR_TOO_MANY_LINKS 1142L

#define ERROR_INVALID_DLL 1154L
#define ERROR_DLL_NOT_FOUND 1157L
#define ERROR_NOT_FOUND 1168L
#define ERROR_CANCELLED 1223L
#define ERROR_NOT_AUTHENTICATED 1244L

#define ERROR_INTERNAL_ERROR 1359L
#define ERROR_FILE_CORRUPT 1392L
#define ERROR_DISK_CORRUPT 1393L
#define ERROR_WRONG_TARGET_NAME 1396L
#define ERROR_NO_SYSTEM_RESOURCES 1450L
#define ERROR_COMMITMENT_LIMIT 1455L
#define ERROR_TIMEOUT 1460L
#define ERROR_EVENTLOG_FILE_CORRUPT 1500L
#define ERROR_LOG_FILE_FULL 1502L
#define ERROR_UNSUPPORTED_TYPE 1630L

#define RPC_S_INVALID_VERS_OPTION 1756L

#define ERROR_RESOURCE_DATA_NOT_FOUND    1812L
#define ERROR_RESOURCE_LANG_NOT_FOUND 1815L
#define ERROR_TAG_NOT_PRESENT 2013L

#define CRYPT_E_NO_MATCH 0x80092009

/* ENDERRORCODES - do not remove this marker, it is consumed by the palsatellite tool */

/******************* error code aliases **************************************/

#define NOERROR 0
#define NO_ERROR 0L

#endif // __PAL_ERROR_H__
