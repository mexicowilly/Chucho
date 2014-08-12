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
#include <chucho/security_policy.h>
#include <chucho/configuration.h>

static void default_text_max(void)
{
    chucho_security_policy* pol;
    size_t tmax;
    chucho_rc rc = chucho_cnf_get_security_policy(&pol);

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get security policy");
    rc = chucho_sec_set_default_text_max(pol, 5);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set default text max");
    rc = chucho_sec_get_default_text_max(pol, &tmax);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get default text max");
    sput_fail_unless(tmax == 5, "default text max is 5");
    rc = chucho_release_security_policy(pol);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release security policy");
}

void run_security_policy_test(void)
{
    sput_enter_suite("security_policy");
    sput_run_test(default_text_max);
    sput_leave_suite();
}
