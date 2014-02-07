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
#include <chucho/size_file_roll_trigger.h>

static void size_file_roll_trigger_test(void)
{
    chucho_file_roll_trigger* trg;
    chucho_rc rc = chucho_create_size_file_roll_trigger(&trg, 512);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create size file roll trigger");
    off_t sz;
    rc = chucho_sftrg_get_max_size(trg, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max size");
    sput_fail_unless(sz == 512, "max size is 512");
    rc = chucho_release_file_roll_trigger(trg);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release file roll trigger");
}

void run_size_file_roll_trigger_test(void)
{
    sput_enter_suite("size_file_roll_trigger");
    sput_run_test(size_file_roll_trigger_test);
    sput_leave_suite();
}
