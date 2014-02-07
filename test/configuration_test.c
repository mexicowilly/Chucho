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
#include <chucho/configuration.h>

#include <stdio.h>

static int my_unknown_handler(const char* key, const char* val)
{
    return 1;
}

static void configuration_test(void)
{
    chucho_rc rc = chucho_cnf_set_allow_default(0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set allow default");
    int val = 1;
    rc = chucho_cnf_allow_default(&val);
    sput_fail_unless(val == 0, "allow default");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get allow default");
    rc = chucho_cnf_set_environment_variable("MY_DOG_HAS_FLEAS");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set environment variable");
    const char* text = NULL;
    rc = chucho_cnf_get_environment_variable(&text);
    sput_fail_if(text == NULL, "null environment variable");
    sput_fail_unless(strcmp(text, "MY_DOG_HAS_FLEAS") == 0, "environment variable");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get environment variable");
    rc = chucho_cnf_set_fallback("hello: there");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set fallback");
    text = NULL;
    rc = chucho_cnf_get_fallback(&text);
    sput_fail_if(text == NULL, "null fallback");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get fallback");
    sput_fail_unless(strcmp(text, "hello: there") == 0, "fallback");
    rc = chucho_cnf_set_file_name("cool.log");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set file name");
    text = NULL;
    rc = chucho_cnf_get_file_name(&text);
    sput_fail_if(text == NULL, "null file name");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get file name");
    sput_fail_unless(strcmp(text, "cool.log") == 0, "file name");
    rc = chucho_cnf_get_loaded_file_name(&text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get loaded file name");
    sput_fail_unless(text == NULL, "empty loaded file name");
    rc = chucho_cnf_set_max_size(500);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set max size");
    size_t sz = 0;
    rc = chucho_cnf_get_max_size(&sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get max size");
    sput_fail_unless(sz == 500, "max size");
    chucho_configuration_style sty = CHUCHO_CONFIGURATION_STYLE_AUTOMATIC;
    rc = chucho_cnf_get_style(&sty);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get style");
    sput_fail_unless(sty == CHUCHO_CONFIGURATION_STYLE_OFF, "style");
    rc = chucho_cnf_set_unknown_handler(&my_unknown_handler);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set unkown handler");
    chucho_unknown_handler_type unk = NULL;
    rc = chucho_cnf_get_unknown_handler(&unk);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get unknown handler");
    sput_fail_unless(unk == &my_unknown_handler, "unknown handler");
}

void run_configuration_test(void)
{
    sput_enter_suite("configuration");
    sput_run_test(configuration_test);
    sput_leave_suite();
}
