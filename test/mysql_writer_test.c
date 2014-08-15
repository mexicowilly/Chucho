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
#include <chucho/mysql_writer.h>
#include <chucho/pattern_formatter.h>

static void mysql_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    const char* text;
    unsigned port;
    chucho_writer* async;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_mysql_writer(&wrt,
                                    fmt,
                                    "192.168.56.101",
                                    "test_user",
                                    "password",
                                    "test",
                                    CHUCHO_DEFAULT_MYSQL_PORT,
                                    801,
                                    chucho_debug_level(),
                                    0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create mysql writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_mwrt_get_database(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_database");
    sput_fail_unless(strcmp(text, "test") == 0, text);
    rc = chucho_mwrt_get_user(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_user");
    sput_fail_unless(strcmp(text, "test_user") == 0, text);
    rc = chucho_mwrt_get_password(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_password");
    sput_fail_unless(strcmp(text, "password") == 0, text);
    rc = chucho_mwrt_get_host(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_host");
    sput_fail_unless(strcmp(text, "192.168.56.101") == 0, text);
    rc = chucho_mwrt_get_port(wrt, &port);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_port");
    sput_fail_unless(port == CHUCHO_DEFAULT_MYSQL_PORT, "chucho_mwrt_get_port");
    rc = chucho_mwrt_get_async_writer(wrt, &async);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_mwrt_get_async_writer");
    rc = chucho_release_writer(async);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release async writer");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release mysql writer");
}

void run_mysql_writer_test(void)
{
    sput_enter_suite("mysql_writer");
    sput_run_test(mysql_writer_test);
    sput_leave_suite();
}
