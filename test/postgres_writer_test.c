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
#include <chucho/postgres_writer.h>
#include <chucho/pattern_formatter.h>

static void postgres_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    const char* text;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_postgres_writer(&wrt, fmt, "postgres://test_user:password@192.168.56.101/postgres");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create postgres writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_pgwrt_get_uri(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_pgwrt_get_uri");
    sput_fail_unless(strcmp(text, "postgres://test_user:password@192.168.56.101/postgres") == 0, text);
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release owrt writer");
}

void run_postgres_writer_test(void)
{
    sput_enter_suite("postgres_writer");
    sput_run_test(postgres_writer_test);
    sput_leave_suite();
}
