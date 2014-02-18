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
#include <chucho/time_file_roller.h>

static void time_file_roller_test(void)
{
    chucho_file_roller* rlr;
    chucho_rc rc = chucho_create_time_file_roller(&rlr, "%d{%d-%H}", 10, NULL);
    const char* pat;
    size_t hist;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create time file roller");
    rc = chucho_trlr_get_file_name_pattern(rlr, &pat);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file name patter");
    sput_fail_unless(strcmp(pat, "%d{%d-%H}") == 0, "pattern is %d{%d-%H}");
    rc = chucho_trlr_get_max_history(rlr, &hist);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max history");
    sput_fail_unless(hist == 10, "history is 10");
    rc = chucho_release_file_roller(rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file roller");
}

void run_time_file_roller_test(void)
{
    sput_enter_suite("time_file_roller");
    sput_run_test(time_file_roller_test);
    sput_leave_suite();
}
