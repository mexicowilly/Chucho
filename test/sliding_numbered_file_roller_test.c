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
#include <chucho/sliding_numbered_file_roller.h>
#include <chucho/error.h>

static void sliding_numbered_file_roller_test(void)
{
    chucho_file_roller* rlr;
    int rc = chucho_create_sliding_numbered_file_roller(&rlr, -7, 10, NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create sliding numbered file roller");
    int idx;
    rc = chucho_snfrlr_get_min_index(rlr, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get min index");
    sput_fail_unless(idx == -7, "min is -7");
    size_t cnt;
    rc = chucho_snfrlr_get_max_count(rlr, &cnt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max count");
    sput_fail_unless(cnt == 10, "max count is 10");
    rc = chucho_snfrlr_get_current_index(rlr, &idx);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get current index");
    sput_fail_unless(idx == -8, "current is -8");
    rc = chucho_release_file_roller(rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release sliding numbered file roller");
}

void run_sliding_numbered_file_roller_test(void)
{
    sput_enter_suite("sliding_numbered_file_roller");
    sput_run_test(sliding_numbered_file_roller_test);
    sput_leave_suite();
}
