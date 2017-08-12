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
#include <chucho/rabbitmq_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/formatted_message_serializer.h>
#include <chucho/message_queue_writer.h>

static void rabbitmq_writer_test(void)
{
    chucho_formatter* fmt;
    chucho_serializer* ser;
    chucho_rc rc;
    chucho_writer* wrt;
    const char* text;
    size_t cmax;

    rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_formatted_message_serializer(&ser);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create formatted message serializer");
    rc = chucho_create_rabbitmq_writer(&wrt, fmt, ser, -1, NULL, "amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc", "logs", NULL);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create rabbitmq writer");
    if (rc != CHUCHO_NO_ERROR)
        return;
    rc = chucho_rmqwrt_get_url(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_rmqwrt_get_url");
    sput_fail_unless(strcmp(text, "amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc") == 0, text);
    rc = chucho_rmqwrt_get_exchange(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_rmqwrt_get_exchange");
    sput_fail_unless(strcmp(text, "logs") == 0, text);
    rc = chucho_rmqwrt_get_routing_key(wrt, &text);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_rmqwrt_get_routing_key");
    sput_fail_unless(strlen(text) == 0, text);
    rc = chucho_mqwrt_get_coalesce_max(wrt, &cmax);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "chucho_zmqwrt_get_coalesce_max");
    sput_fail_unless(cmax == 25, "Coalesce max");
     rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release rmqwrt writer");
}

void run_rabbitmq_writer_test(void)
{
    sput_enter_suite("rabbitmq_writer");
    sput_run_test(rabbitmq_writer_test);
    sput_leave_suite();
}
