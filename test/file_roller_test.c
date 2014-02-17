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
#include <chucho/numbered_file_roller.h>

static void file_roller_test(void)
{
    chucho_file_roller* rlr;
    chucho_rc rc = chucho_create_numbered_file_roller(&rlr, 1, 10, NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create numbered file roller");
    char buf[1];
    size_t count;
    rc = chucho_rlr_get_active_file_name(rlr, buf, 1, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get active file name");
    sput_fail_unless(count == 1, "length is 1");
    sput_fail_unless(buf[0] == 0, "name is empty");
    chucho_file_compressor* cmp;
    rc = chucho_rlr_get_file_compressor(rlr, &cmp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file compressor");
    sput_fail_unless(cmp == NULL, "compressor is NULL");
    rc = chucho_release_file_roller(rlr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release numbered file roller");
}

void run_file_roller_test(void)
{
    sput_enter_suite("file_roller");
    sput_run_test(file_roller_test);
    sput_leave_suite();
}
