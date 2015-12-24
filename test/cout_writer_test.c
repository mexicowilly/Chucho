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
#include <chucho/cout_writer.h>
#include <chucho/pattern_formatter.h>

static void cout_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_writer* wrt;

    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_cout_writer(&wrt, fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create cout writer");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release cout writer");
}

void run_cout_writer_test(void)
{
    sput_enter_suite("cout_writer");
    sput_run_test(cout_writer_test);
    sput_leave_suite();
}
