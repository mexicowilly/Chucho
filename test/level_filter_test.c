/*
 * Copyright 2013-2017 Will Mason
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
#include <chucho/level_filter.h>

static void level_filter_test(void)
{
    chucho_filter* flt;
    chucho_rc rc = chucho_create_level_filter(&flt,
                                        chucho_error_level(),
                                        CHUCHO_FILTER_RESULT_DENY,
                                        CHUCHO_FILTER_RESULT_ACCEPT);
    const chucho_level* lvl;
    chucho_filter_result res;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create level filter");
    rc = chucho_lflt_get_level(flt, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level");
    sput_fail_unless(lvl == chucho_error_level(), "level is ERROR");
    rc = chucho_lflt_get_on_match(flt, &res);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get on match");
    sput_fail_unless(res == CHUCHO_FILTER_RESULT_DENY, "on match is DENY");
    rc = chucho_lflt_get_on_mismatch(flt, &res);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get on mismatch");
    sput_fail_unless(res == CHUCHO_FILTER_RESULT_ACCEPT, "on mismatch is ACCEPT");
    rc = chucho_release_filter(flt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release level filter");
}

void run_level_filter_test(void)
{
    sput_enter_suite("level_filter");
    sput_run_test(level_filter_test);
    sput_leave_suite();
}
