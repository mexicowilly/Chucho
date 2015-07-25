@echo off

rem
rem Copyright 2013-2015 Will Mason
rem 
rem    Licensed under the Apache License, Version 2.0 (the "License");
rem    you may not use this file except in compliance with the License.
rem    You may obtain a copy of the License at
rem 
rem      http://www.apache.org/licenses/LICENSE-2.0
rem 
rem    Unless required by applicable law or agreed to in writing, software
rem    distributed under the License is distributed on an "AS IS" BASIS,
rem    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
rem    See the License for the specific language governing permissions and
rem    limitations under the License.
rem

setlocal

set args=0
for %%i in (%*) do set /a args+=1
if %args%==0 goto usage

set HOST=localhost
set MESSAGES_PER_SECOND=1
set COUNT=1

:loopargs
    if "%1"=="" goto continue
    if %1==-c set COUNT=%2
    if %1==-h set HOST=%2
    if %1==-m set TOTAL_MINUTES=%2
    if %1==-p set PROGRAM=%~2
    if %1==-v set MESSAGES_PER_SECOND=%2
    shift
    shift
    goto loopargs
:continue

echo Client program: %PROGRAM%
echo Host: %HOST%
echo Messages/second: %MESSAGES_PER_SECOND%
echo Total minutes: %TOTAL_MINUTES%
echo Number of clients: %COUNT%

for /L %%i in (1,1,%COUNT%) do (
    start "" /B /WAIT "%PROGRAM%" --log client.%%i --messages_per_second %MESSAGES_PER_SECOND% --total_minutes %TOTAL_MINUTES% --host %HOST%
)

exit /B
    
:usage
    echo Accepted arguments:
    echo     -c arg : The number of clients to run
    echo     -h arg : The host to which to connect
    echo     -m arg : The number of minutes to run
    echo     -p arg : The client program
    echo     -v arg : The number of messages to write per second
