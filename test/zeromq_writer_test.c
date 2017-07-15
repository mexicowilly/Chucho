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
#include <chucho/zeromq_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/formatted_message_serializer.h>

static void zeromq_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_serializer* ser;
    chucho_rc rc;
    chucho_writer* wrt;
    unsigned char prefix[6] = { 1, 2, 3, 4, 5, 6 };
    const char* text;
    const unsigned char* found_pfx;
    size_t found_len;

    rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_formatted_message_serializer(&ser);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create formatted message serializer");
    rc = chucho_create_zeromq_writer(&wrt, fmt, ser, NULL, "tcp://127.0.0.1:7777", prefix, 6);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create zeromq writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_zmqwrt_get_endpoint(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_zmqwrt_get_endpoint");
    sput_fail_unless(strcmp(text, "tcp://127.0.0.1:7777") == 0, text);
    rc = chucho_zmqwrt_get_prefix(wrt, &found_pfx, &found_len);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_zmqwrt_get_prefix");
    sput_fail_unless(found_len == 6, "Found prefix length");
    sput_fail_unless(memcmp(prefix, found_pfx, found_len) == 0, "Found prefix");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release owrt writer");
}

void run_zeromq_writer_test(void)
{
    sput_enter_suite("zeromq_writer");
    sput_run_test(zeromq_writer_test);
    sput_leave_suite();
}
