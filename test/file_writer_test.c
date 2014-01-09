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
#include <chucho/file_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/error.h>

static void file_writer_test(void)
{
    chucho_formatter* fmt;
    int rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_writer* wrt;
    rc = chucho_create_file_writer(&wrt, fmt, "hello", CHUCHO_ON_START_APPEND, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create file writer");
    const char* name;
    rc = chucho_fwrt_get_file_name(wrt, &name);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_fwrt_get_file_name");
    sput_fail_unless(strcmp(name, "hello") == 0, name);
    int flsh;
    rc = chucho_fwrt_get_flush(wrt, &flsh);
    sput_fail_unless(flsh == 0, "should not flush");
    chucho_on_start on;
    rc = chucho_fwrt_get_on_start(wrt, &on);
    sput_fail_unless(on == CHUCHO_ON_START_APPEND, "should append");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file writer");
}

void run_file_writer_test(void)
{
    sput_enter_suite("file_writer");
    sput_run_test(file_writer_test);
    sput_leave_suite();
}
