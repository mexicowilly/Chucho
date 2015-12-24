/*
 * Copyright 2013-2016 Will Mason
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
#include <chucho/file_descriptor_writer.h>
#include <chucho/pattern_formatter.h>

static void file_descriptor_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    int flsh;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_file_descriptor_writer(&wrt, fmt, 75, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create file descriptor writer");
    rc = chucho_fdwrt_get_flush(wrt, &flsh);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "file descriptor writer get flush");
    sput_fail_unless(flsh == 0, "should not flush");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file descriptor writer");
}

void run_file_descriptor_writer_test(void)
{
    sput_enter_suite("file_descriptor_writer");
    sput_run_test(file_descriptor_writer_test);
    sput_leave_suite();
}
