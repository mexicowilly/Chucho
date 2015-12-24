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
#include <chucho/cout_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/duplicate_message_filter.h>

static void writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    chucho_formatter* got_fmt;
    size_t flt_count;
    chucho_filter* dup;
    chucho_filter* flts[1];

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_cout_writer(&wrt, fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create cout writer");
    rc = chucho_wrt_get_formatter(wrt, &got_fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get formatter");
    // Can't do anything with the formatter
    rc = chucho_release_formatter(got_fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release formatter");
    rc = chucho_wrt_get_filters(wrt, NULL, 0, &flt_count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "rc is CHUCHO_NULL_POINTER");
    sput_fail_unless(flt_count == 0, "filter count is zero");
    rc = chucho_create_duplicate_message_filter(&dup);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create duplicate message filter");
    rc = chucho_wrt_add_filter(wrt, dup);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add filter");
    rc = chucho_wrt_get_filters(wrt, flts, 1, &flt_count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get filters");
    sput_fail_unless(flt_count == 1, "filter count is 1");
    rc = chucho_release_filter(flts[0]);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release filter");
    rc = chucho_wrt_clear_filters(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "clear filters");
    rc = chucho_wrt_get_filters(wrt, NULL, 0, &flt_count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "rc is CHUCHO_NULL_POINTER");
    sput_fail_unless(flt_count == 0, "filter count is zero");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release cout writer");
}

void run_writer_test(void)
{
    sput_enter_suite("writer");
    sput_run_test(writer_test);
    sput_leave_suite();
}
