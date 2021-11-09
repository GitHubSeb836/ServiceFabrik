:: ------------------------------------------------------------
:: Copyright (c) Microsoft Corporation.  All rights reserved.
:: Licensed under the MIT License (MIT). See License.txt in the repo root for license information.
:: ------------------------------------------------------------

@echo off
setlocal EnableExtensions EnableDelayedExpansion

set CurDir=%~dp0

:: Ensure we are running from FabricUnitTests
:: We cannot run it from Git source path since the corext environment
:: doesn't provide  build type (debug/retail) info in its environment.
:: So, we aren't sure if we need to go to debug-amd64\bin\FabricUnitTests or retail-amd64\bin\FabricUnitTests
:: So, executing from the destination directory itself.
echo %CurDir% >%temp%\copycitlogs.curdir.txt
find /I "FabricUnitTests" %temp%\copycitlogs.curdir.txt >nul
set ev=%errorlevel%
del %temp%\copycitlogs.curdir.txt
if %ev% equ 1 goto RunFromFabricUnitTestsDirMsg

set CurDir=%~dp0
:: Remove the last backslash
set CurDir=%CurDir:~0,-1%
for %%d in (%CurDir%) do set ParentDir=%%~dpd

set _NTTREE=%ParentDir:~0,-1%%

if "%1"=="" goto :PrintUsage
if "%1"=="/?" goto :PrintUsage
if "%1"=="-?" goto :PrintUsage

:: ===========================================================================
::Parse command line arguments
::
set CopyOptions=
set CopyMode=
set TestList=
set CloudMode=

set params=%*

:ParseArguments
    setlocal EnableDelayedExpansion
    for /f "tokens=1* delims= " %%i in ("%params%") do (
        set ProcessArgument=true
        call :ProcessArgument %%i
        set params=%%j
    )
    if "%ProcessArgument%" == "false" ( exit /b 1 )
if not "%params%" == "" goto :ParseArguments

if "%CopyMode%" == "" (
     goto :PrintUsage 
     )

call :RunTests 
goto :EOF

:ProcessArgument 
set argument=%*
set optionName=
set optionValue=

for /f "tokens=1* delims=:" %%i in ("%argument%") do (
    set optionName=%%i
    set optionValue=%%j
)

if /i "%optionName%"=="/dest" (
    if "%CopyOptions%"=="" (
       set CopyOptions=:%optionValue%
       goto :EOF
    )

    set CopyOptions=!CopyOptions!,%optionValue%
    goto :EOF
) 

if /i "%optionName%"=="/failures" (
    set CopyMode=/copyFailures
    goto :EOF
) 
    
if /i "%optionName%"=="/tests" (
    set CopyMode=/copyLogs
    set TestList=%argument%
    goto :EOF
)

if /i "%optionName%"=="/cloud" (
    set CloudMode=1
    if not "%optionValue%"=="" (
        if "%CopyOptions%"=="" (
            set CopyOptions=:%optionValue%
        ) else (
            set CopyOptions=!CopyOptions!,%optionValue%
        )
    )
    goto :EOF
)

if /i "%optionName%"=="/crash" (
    if "%CopyOptions%"=="" (
       set CopyOptions=:crash
       goto :EOF
    ) 

    set CopyOptions=!CopyOptions!,crash
    goto :EOF
) 

if /i "%optionName%"=="/data" (
    if "%CopyOptions%"=="" (
       set CopyOptions=:data
       goto :EOF
    ) 

    set CopyOptions=!CopyOptions!,data
    goto :EOF
) 

set ProcessArgument=false
goto :PrintUsage 

:RunTests

if "%CloudMode%"=="1" (
    if "%CopyMode%"=="/copyFailures" (
        if "%CopyOptions%"=="" (
            set CopyOptions=:failures
        ) else (
            set CopyOptions=!CopyOptions!,failures
        )
    )
    set CopyMode=/copyCloudLogs
)

set BINARIES_DIR=%_NTTREE%\FabricUnitTests
set path=%path%;%BINARIES_DIR%

pushd %BINARIES_DIR%
RunTests.exe %CopyMode%%CopyOptions% %TestList%
goto :EOF

:: ===========================================================================
:: Print the usage 
::
:PrintUsage

echo.
echo This script copies Windows Fabric CIT Logs through RunTests.exe
echo.
echo Usage: CopyCitLogs.cmd [/dest:destination folder path] [/tests:listOfTests OR /failures] [/crash] [/data] [/cloud[:runId=xxxx]]
echo.
echo where,
echo.
echo    /tests          The list of tests for which logs will be copied.
echo.
echo    /failures       Uses the failure file generated by RunTests to copy logs for all failed tests.
echo.
echo    /dest           The destination folder where logs will be copied. It must exist.
echo                    If not specified, environment variable "CopyLogsFolderName" is read.
echo                    If not set, \\winfabfs\scratch2\WinFabCITLogs folder is used.
echo.
echo    /crash          Specifies that the symbols and any existing associated dumps should be copied.
echo.
echo    /data           Specifies that the data directory should be copied.
echo.
echo    /cloud          Specifies that logs are downloaded for cloud mode CIT run.
echo                    If runId=xxxx is provided, logs for the specified CIT run
echo                    are downloaded. Otherwise logs for the last cloud CIT run
echo                    submitted from current enlistment are downloaded.
echo.

endlocal
goto :EOF

:: ===========================================================================
:: Displays a message to run the script from FabricUnitTests folder
::
:RunFromFabricUnitTestsDirMsg
echo.
echo Please run this script from FabricUnitTests folder under bin
echo E.g. from \rdnext\WindowsFabric\out\debug-amd64\bin\FabricUnitTests
echo.
echo For detailed usage, please type '%~nx0 /?'
echo.

endlocal
goto :EOF