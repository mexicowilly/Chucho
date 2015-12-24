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

static void integer_range(void)
{
    chucho_security_policy* pol;
    long long low;
    long long high;
    chucho_rc rc = chucho_cnf_get_security_policy(&pol);

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get security policy");
    rc = chucho_sec_set_integer(pol, "int", -7, 7);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set integer");
    rc = chucho_sec_get_integer_range(pol, "int", &low, &high);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get integer range");
    sput_fail_unless(low == -7, "low equals -7");
    sput_fail_unless(high == 7, "high equals 7");
    rc = chucho_release_security_policy(pol);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release security policy");
}

static void override_integer(void)
{
    chucho_security_policy* pol;
    long long low;
    long long high;
    chucho_rc rc = chucho_cnf_get_security_policy(&pol);

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get security policy");
    rc = chucho_sec_set_integer(pol, "oint", -7, 7);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set integer");
    rc = chucho_sec_set_integer(pol, "oint", -500, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set integer");
    rc = chucho_sec_get_integer_range(pol, "oint", &low, &high);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get integer range");
    sput_fail_unless(low == -7, "low equals -7");
    sput_fail_unless(high == 7, "high equals 7");
    rc = chucho_sec_override_integer(pol, "oint", -500, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "override integer");
    rc = chucho_sec_get_integer_range(pol, "oint", &low, &high);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get integer range");
    sput_fail_unless(low == -500, "low equals -500");
    sput_fail_unless(high == 0, "high equals 0");
    rc = chucho_release_security_policy(pol);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release security policy");
}

static void override_text(void)
{
    chucho_security_policy* pol;
    size_t tmax;
    chucho_rc rc = chucho_cnf_get_security_policy(&pol);

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get security policy");
    rc = chucho_sec_set_text(pol, "otext", 500);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set text");
    rc = chucho_sec_set_text(pol, "otext", 501);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set text");
    rc = chucho_sec_get_text_max(pol, "otext", &tmax);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get text max");
    sput_fail_unless(tmax == 500, "tmax equals 500");
    rc = chucho_sec_override_text(pol, "otext", 501);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "override text");
    rc = chucho_sec_get_text_max(pol, "otext", &tmax);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get text max");
    sput_fail_unless(tmax == 501, "tmax equals 501");
    rc = chucho_release_security_policy(pol);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release security policy");
}

static void text(void)
{
    chucho_security_policy* pol;
    size_t tmax;
    chucho_rc rc = chucho_cnf_get_security_policy(&pol);

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get security policy");
    rc = chucho_sec_set_text(pol, "text", 500);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set text");
    rc = chucho_sec_get_text_max(pol, "text", &tmax);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get text max");
    sput_fail_unless(tmax == 500, "tmax equals 500");
    rc = chucho_release_security_policy(pol);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release security policy");
}

void run_security_policy_test(void)
{
    sput_enter_suite("security_policy");
    sput_run_test(default_text_max);
    sput_run_test(integer_range);
    sput_run_test(override_integer);
    sput_run_test(override_text);
    sput_run_test(text);
    sput_leave_suite();
}
