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
#include <chucho/logger.h>
#include <chucho/file_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/error.h>

static chucho_logger* lgr;

static void set_up(void)
{
    int rc = chucho_get_logger(&lgr, "log_macro");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get logger log_macro");
    chucho_formatter* fmt;
    rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_writer* wrt;
    rc = chucho_create_file_writer(&wrt, fmt, "log_macro.log", CHUCHO_ON_START_TRUNCATE, 1);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create file writer");
    rc = chucho_lgr_add_writer(lgr, wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add writer");
}

static void tear_down(void)
{
    int rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger log_macro");
    rc = chucho_remove_unused_loggers();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove unused loggers");
}

void run_log_macro_test(void)
{
    sput_enter_suite_fixture("log macros", set_up, tear_down);
}
