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
#include <chucho/async_writer.h>
#include <chucho/file_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/level.h>

static void async_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_writer* wrt;
    chucho_writer* async;
    const chucho_level* dt;
    const char* warn;
    size_t sz;
    const char* fname;
    int flsh;

    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_file_writer(&wrt, fmt, "hello", CHUCHO_ON_START_APPEND, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create file writer");
    rc = chucho_create_async_writer(&async, wrt, 777, chucho_warn_level(), 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create async writer");
    rc = chucho_aswrt_get_flush_on_destruct(async, &flsh);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get flush on destruct");
    sput_fail_unless(flsh == 0, "flush on destruct is false");
    rc = chucho_aswrt_get_discard_threshold(async, &dt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get discard threshold");
    rc = chucho_lvl_get_name(dt, &warn);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "level get name");
    sput_fail_unless(strcmp(warn, "WARN") == 0, "level is warn");
    rc = chucho_aswrt_get_queue_capacity(async, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get queue capacity");
    sput_fail_unless(sz == 777, "capacity is 777");
    rc = chucho_aswrt_get_queue_size(async, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get queue size");
    sput_fail_unless(sz == 0, "size is 0");
    rc = chucho_aswrt_get_writer(async, &wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get writer");
    rc = chucho_fwrt_get_file_name(wrt, &fname);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file name");
    sput_fail_unless(strcmp(fname, "hello") == 0, "file name is hello");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file writer");
    rc = chucho_release_writer(async);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release async writer");
}

void run_async_writer_test(void)
{
    sput_enter_suite("async_writer");
    sput_run_test(async_writer_test);
    sput_leave_suite();
}
