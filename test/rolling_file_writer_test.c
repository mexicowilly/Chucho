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
#include <chucho/rolling_file_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/size_file_roll_trigger.h>
#include <chucho/numbered_file_roller.h>
#include <chucho/time_file_roller.h>
#include <chucho/error.h>

static void rolling_file_name(void)
{
    chucho_formatter* fmt;
    int rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_file_roller* rlr;
    rc = chucho_create_numbered_file_roller(&rlr, 1, 10, NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create numbered file roller");
    chucho_file_roll_trigger* trg;
    rc = chucho_create_size_file_roll_trigger(&trg, 512);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create size file roll trigger");
    chucho_writer* wrt;
    rc = chucho_create_rolling_file_writer(&wrt, fmt, rlr, trg, NULL, CHUCHO_ON_START_APPEND, 0);
    sput_fail_unless(rc == CHUCHO_INVALID_ARGUMENT, "create invalid rolling file writer");
    rc = chucho_create_rolling_file_writer(&wrt, fmt, rlr, trg, "hello.log", CHUCHO_ON_START_APPEND, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create rolling file writer");
    const char* name;
    rc = chucho_fwrt_get_file_name(wrt, &name);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_fwrt_get_file_name");
    sput_fail_unless(strcmp(name, "hello.log") == 0, name);
    int flsh;
    rc = chucho_fwrt_get_flush(wrt, &flsh);
    sput_fail_unless(flsh == 0, "should not flush");
    chucho_on_start on;
    rc = chucho_fwrt_get_on_start(wrt, &on);
    sput_fail_unless(on == CHUCHO_ON_START_APPEND, "should append");
    chucho_file_roller* got_rlr;
    rc = chucho_rfwrt_get_file_roller(wrt, &got_rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file roller");
    int idx;
    rc = chucho_nrlr_get_min_index(got_rlr, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get min index");
    sput_fail_unless(idx == 1, "min is 1");
    rc = chucho_nrlr_get_max_index(got_rlr, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max index");
    sput_fail_unless(idx == 10, "max is 10");
    rc = chucho_release_file_roller(got_rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file roller");
    chucho_file_roll_trigger* got_trg;
    rc = chucho_rfwrt_get_file_roll_trigger(wrt, &got_trg);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file roll trigger");
    off_t sz;
    rc = chucho_sftrg_get_max_size(got_trg, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max size");
    sput_fail_unless(sz == 512, "max size is 512");
    rc = chucho_release_file_roll_trigger(got_trg);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file roll trigger");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file writer");
}

static void rolling_file_no_name(void)
{
    chucho_formatter* fmt;
    int rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_file_roller* rlr;
    rc = chucho_create_time_file_roller(&rlr, "%d{%d-%H}", 10, NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create time file roller");
    chucho_writer* wrt;
    rc = chucho_create_rolling_file_writer(&wrt, fmt, rlr, NULL, NULL, CHUCHO_ON_START_APPEND, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create rolling file writer");
    int flsh;
    rc = chucho_fwrt_get_flush(wrt, &flsh);
    sput_fail_unless(flsh == 0, "should not flush");
    chucho_on_start on;
    rc = chucho_fwrt_get_on_start(wrt, &on);
    sput_fail_unless(on == CHUCHO_ON_START_APPEND, "should append");
    chucho_file_roller* got_rlr;
    rc = chucho_rfwrt_get_file_roller(wrt, &got_rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file roller");
    const char* pat;
    rc = chucho_trlr_get_file_name_pattern(got_rlr, &pat);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file name patter");
    sput_fail_unless(strcmp(pat, "%d{%d-%H}") == 0, "pattern is %d{%d-%H}");
    size_t hist;
    rc = chucho_trlr_get_max_history(got_rlr, &hist);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max history");
    sput_fail_unless(hist == 10, "history is 10");
    rc = chucho_release_file_roller(got_rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file roller");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file writer");
}

void run_rolling_file_writer_test(void)
{
    sput_enter_suite("rolling_file_writer");
    sput_run_test(rolling_file_name);
    sput_run_test(rolling_file_no_name);
    sput_leave_suite();
}

