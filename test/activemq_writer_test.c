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
#include <chucho/activemq_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/formatted_message_serializer.h>

#include <stdio.h>

static void activemq_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_serializer* ser;
    chucho_rc rc;
    chucho_writer* wrt;
    const char* text;
    chucho_activemq_consumer_type tp;

    rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_formatted_message_serializer(&ser);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create formatted message serializer");
    rc = chucho_create_activemq_writer(&wrt, fmt, ser, NULL, "tcp://127.0.0.1:61616", CHUCHO_ACTIVEMQ_QUEUE, "chunky_monkey");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create activemq writer");
    printf("rc %i\n", rc);
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_amqwrt_get_broker(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_amqwrt_get_broker");
    sput_fail_unless(strcmp(text, "tcp://127.0.0.1:61616") == 0, text);
    rc = chucho_amqwrt_get_consumer_type(wrt, &tp);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_amqwrt_get_consumer_type");
    sput_fail_unless(tp == CHUCHO_ACTIVEMQ_QUEUE, "Found queue");
    rc = chucho_amqwrt_get_topic_or_queue(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_amqwrt_get_topic_or_queue");
    sput_fail_unless(strcmp(text, "chunky_monkey") == 0, text);
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release amqwrt writer");
}

void run_activemq_writer_test(void)
{
    sput_enter_suite("activemq_writer");
    sput_run_test(activemq_writer_test);
    sput_leave_suite();
}