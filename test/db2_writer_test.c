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
#include <chucho/db2_writer.h>
#include <chucho/pattern_formatter.h>

static void db2_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    const char* text;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_db2_writer(&wrt, fmt, "chucho", "db2inst1", "db2inst1");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create db2 writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_db2wrt_get_database(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_db2wrt_get_database");
    sput_fail_unless(strcmp(text, "chucho") == 0, text);
    rc = chucho_db2wrt_get_user(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_db2wrt_get_user");
    sput_fail_unless(strcmp(text, "db2inst1") == 0, text);
    rc = chucho_db2wrt_get_password(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_db2wrt_get_password");
    sput_fail_unless(strcmp(text, "db2inst1") == 0, text);
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release db2wrt writer");
}

void run_db2_writer_test(void)
{
    sput_enter_suite("db2_writer");
    sput_run_test(db2_writer_test);
    sput_leave_suite();
}
