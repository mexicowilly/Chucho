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
#include <chucho/email_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/level_threshold_email_trigger.h>

static void email_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_rc rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    chucho_writer* wrt;
    const char* text;
    const char* tos[] = { "blahblah@blah.com", "hothere@ok.org", NULL };
    chucho_email_trigger* trg = NULL;
    size_t sz;
    chucho_email_connection_type tp;
    unsigned port;
    const char** found_tos;
    size_t count;
    const chucho_level* lvl;
    int lvl_val;
    int err_lvl_val;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_level_threshold_email_trigger(&trg, chucho_error_level());
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create error level email trigger");
    rc = chucho_create_email_writer(&wrt,
                                    fmt,
                                    "mail.dummy.com",
                                    CHUCHO_EMAIL_CONNECTION_CLEAR,
                                    tos,
                                    "chunky@monkeyballs.com",
                                    "%c",
                                    trg,
                                    "scrumpy",
                                    "lumpy",
                                    765,
                                    7000);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create email writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_ewrt_get_buffer_capacity(wrt, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_buffer_capacity");
    sput_fail_unless(sz == 7000, "capacity is not 7000");
    rc = chucho_ewrt_get_buffer_size(wrt, &sz);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_buffer_size");
    sput_fail_unless(sz == 0, "size is not 0");
    rc = chucho_ewrt_get_connection_type(wrt, &tp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_connection_type");
    sput_fail_unless(tp == CHUCHO_EMAIL_CONNECTION_CLEAR, "connection type is not CHUCHO_EMAIL_CONNECTION_CLEAR");
    rc = chucho_ewrt_get_from(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_from");
    sput_fail_unless(strcmp(text, "chunky@monkeyballs.com") == 0, text);
    rc = chucho_ewrt_get_host(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_host");
    sput_fail_unless(strcmp(text, "mail.dummy.com") == 0, text);
    rc = chucho_ewrt_get_password(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_password");
    sput_fail_unless(strcmp(text, "lumpy") == 0, text);
    rc = chucho_ewrt_get_port(wrt, &port);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_port");
    sput_fail_unless(port == 765, "the port is 765");
    rc = chucho_ewrt_get_subject(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_subject");
    sput_fail_unless(strcmp(text, "%c") == 0, text);
    rc = chucho_ewrt_get_to(wrt, NULL, 0, &count);
    sput_fail_unless(rc == CHUCHO_INSUFFICIENT_BUFFER, "chucho_ewrt_get_to pre-flight");
    sput_fail_unless(count == 2, "to count does equal 2");
    found_tos = malloc(sizeof(char*) * count);
    rc = chucho_ewrt_get_to(wrt, &found_tos, 2, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_to");
    sput_fail_unless(strcmp(found_tos[0], "blahblah@blah.com") == 0, found_tos[0]);
    sput_fail_unless(strcmp(found_tos[1], "hothere@ok.org") == 0, found_tos[1]);
    free(found_tos);
    rc = chucho_ewrt_get_trigger(wrt, &trg);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_trigger");
    rc = chucho_ltetrg_get_level(trg, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ltetrg_get_level");
    rc = chucho_lvl_get_value(lvl, &lvl_val);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_lvl_get_value");
    rc = chucho_lvl_get_value(chucho_error_level(), &err_lvl_val);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_lvl_get_value");
    sput_fail_unless(lvl_val == err_lvl_val, "The trigger's level is error");
    rc = chucho_release_email_trigger(trg);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_release_email_trigger");
    rc = chucho_ewrt_get_user(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_ewrt_get_user");
    sput_fail_unless(strcmp(text, "scrumpy") == 0, text);
}

void run_email_writer_test(void)
{
    sput_enter_suite("email_writer");
    sput_run_test(email_writer_test);
    sput_leave_suite();
}