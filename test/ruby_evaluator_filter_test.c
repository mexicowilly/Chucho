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
#include <chucho/ruby_evaluator_filter.h>

static void ruby_evaluator_filter_test(void)
{
    chucho_filter* flt;
    chucho_rc rc = chucho_create_ruby_evaluator_filter(&flt, "$logger == 'will'");
    const char* exp;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create ruby evaluator filter");
    rc = chucho_rbflt_get_expression(flt, &exp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get expression");
    sput_fail_unless(strcmp("$logger == 'will'", exp) == 0, "expression is $logger == 'will'");
    rc = chucho_release_filter(flt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release ruby evaluator filter");
}

void run_ruby_evaluator_filter_test(void)
{
    sput_enter_suite("ruby_evaluator_filter");
    sput_run_test(ruby_evaluator_filter_test);
    sput_leave_suite();
}

