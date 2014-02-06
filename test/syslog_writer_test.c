/*
 * Copyright 2013-2014 Will Mason
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
#include <chucho/syslog_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/error.h>

static void local_syslog_writer_test(void)
{
    chucho_formatter* fmt;
    int rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_writer* wrt;
    rc = chucho_create_local_syslog_writer(&wrt, fmt, CHUCHO_SYSLOG_FACILITY_LOCAL0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create syslog writer");
    chucho_syslog_facility fcl;
    rc = chucho_slwrt_get_facility(wrt, &fcl);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get facility");
    sput_fail_unless(fcl == CHUCHO_SYSLOG_FACILITY_LOCAL0, "facility is local0");
    const char* name;
    rc = chucho_slwrt_get_host_name(wrt, &name);
    sput_fail_unless(rc == CHUCHO_NO_SUCH_VALUE, "no host name");
    unsigned port;
    rc = chucho_slwrt_get_port(wrt, &port);
    sput_fail_unless(rc == CHUCHO_NO_SUCH_VALUE, "no port");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release syslog writer");
}

void run_syslog_writer_test(void)
{
    sput_enter_suite("syslog_writer");
    sput_run_test(local_syslog_writer_test);
    sput_leave_suite();
}
