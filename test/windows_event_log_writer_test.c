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

#include "sput.h"
#include <chucho/windows_event_log_writer.h>
#include <chucho/pattern_formatter.h>
#include <windows.h>

static void windows_event_log_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    const char* text;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    SetEnvironmentVariableA("CHUCHO_EVENT_LOG_DLL", CHUCHO_EVENT_LOG_DLL);
    rc = chucho_create_windows_event_log_writer(&wrt,
                                                fmt,
                                                "Application",
                                                "ChuchoTest",
                                                NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create windows_event_log writer");
    rc = chucho_welwrt_get_host(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_welwrt_get_host");
    sput_fail_unless(text == NULL, "host should be NULL");
    rc = chucho_welwrt_get_log(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_welwrt_get_log");
    sput_fail_unless(strcmp(text, "Application") == 0, text);
    rc = chucho_welwrt_get_source(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_welwrt_get_password");
    sput_fail_unless(strcmp(text, "ChuchoTest") == 0, text);
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release welwrt writer");
}

void run_windows_event_log_writer_test(void)
{
    sput_enter_suite("windows_event_log_writer");
    sput_run_test(windows_event_log_writer_test);
    sput_leave_suite();
}
