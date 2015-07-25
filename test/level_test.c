/*
 * Copyright 2013-2015 Will Mason
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
#include <chucho/level.h>

static void level_test(void)
{
    const chucho_level* lvl;
    chucho_rc rc = chucho_add_level(&lvl, "monkey", 700, CHUCHO_SYSLOG_SEVERITY_CRITICAL);
    const chucho_level* got;
    const char* name;
    int val;
    chucho_syslog_severity sev;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add level");
    rc = chucho_get_level(&got, "MONKEY");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level");
    sput_fail_unless(lvl == got, "level pointer is the same");
    rc = chucho_lvl_get_name(lvl, &name);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get name");
    sput_fail_unless(strcmp(name, "monkey") == 0, "name is monkey");
    rc = chucho_lvl_get_value(lvl, &val);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get value");
    sput_fail_unless(val == 700, "value is 700");
    rc = chucho_lvl_get_syslog_severity(lvl, &sev);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get syslog severity");
    sput_fail_unless(sev == CHUCHO_SYSLOG_SEVERITY_CRITICAL, "syslog severity is CHUCHO_SYSLOG_SEVERITY_CRITICAL");
}

void run_level_test(void)
{
    sput_enter_suite("level");
    sput_run_test(level_test);
    sput_leave_suite();
}
