/*
 * Copyright 2013-2017 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "daemon.hpp"
#include "is_shut_down.hpp"
#include "log.hpp"
#include <windows.h>

namespace
{

const char* SERVICE_NAME = "Chucho";

SERVICE_STATUS_HANDLE status_handle = 0;
DWORD current_state = SERVICE_STOPPED;
std::function<void()> run;

void report_service_status(DWORD state,
                           DWORD waitHint = 0,
                           DWORD checkPoint = 0,
                           DWORD controlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN)
{
    SERVICE_STATUS status;
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status.dwCurrentState = state;
    status.dwControlsAccepted = controlsAccepted;
    status.dwWin32ExitCode = NO_ERROR;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = checkPoint;
    status.dwWaitHint = waitHint;
    SetServiceStatus(status_handle, &status);
}

void WINAPI service_control_handler(DWORD control)
{
    if (control == SERVICE_CONTROL_STOP ||
        control == SERVICE_CONTROL_SHUTDOWN)
    {
        report_service_status(SERVICE_STOP_PENDING);
        chucho::server::is_shut_down = true;
        report_service_status(SERVICE_STOPPED);
        CHUCHO_INFO_STR(chucho::logger::get("chuchod"), "Service stopped");
    }
    else if (control == SERVICE_CONTROL_INTERROGATE)
    {
        report_service_status(current_state);
    }
}

void WINAPI service_main(DWORD, char**)
{
    auto lgr = chucho::logger::get("chuchod");
    try
    {
        status_handle = RegisterServiceCtrlHandlerA(SERVICE_NAME,
                                                    service_control_handler);
        if (status_handle == 0)
        {
            CHUCHO_ERROR(lgr, "Unable to register service control handler (" <<
                GetLastError() << ')');
        }
        else
        {
            report_service_status(SERVICE_START_PENDING, 60000, 1, 0);
            report_service_status(SERVICE_RUNNING);
            CHUCHO_INFO_STR(lgr, "Service running");
            run();
        }
    }
    catch (std::exception& e)
    {
        CHUCHO_ERROR(lgr, "Exception in service_main: " << e.what());
    }
}

}

namespace chucho
{

namespace server
{

namespace daemon
{

void possess(std::function<void()> func)
{
    CHUCHO_DEBUG_STR(chucho::logger::get("chuchod"), "About to start service control dispatcher");
    run = func;
    SERVICE_TABLE_ENTRYA entry[] =
    {
        { const_cast<char*>(SERVICE_NAME), service_main },
        { nullptr, nullptr }
    };
    if (!StartServiceCtrlDispatcherA(entry))
        throw exception("Unable to start service dispatcher");
}

}

}

}
