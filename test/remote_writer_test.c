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
#include <chucho/remote_writer.h>
#include <chucho/error.h>

static void remote_writer_test(void)
{
    chucho_writer* wrt;
    int rc = chucho_create_remote_writer(&wrt, "motherboy", 19567, 12001);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create remote writer");
    const char* host;
    rc = chucho_rwrt_get_host(wrt, &host);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get host");
    sput_fail_unless(strcmp(host, "motherboy") == 0, "host is motherboy");
    unsigned port;
    rc = chucho_rwrt_get_port(wrt, &port);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get port");
    sput_fail_unless(port == 19567, "port is 19567");
    size_t cache;
    rc = chucho_rwrt_get_unsent_cache_max(wrt, &cache);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get unsent cache max");
    sput_fail_unless(cache == 12001, "unsent cache max is 12001");
    rc = chucho_rwrt_get_unsent_cache_size(wrt, &cache);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get unsent cache size");
    sput_fail_unless(cache == 0, "unsent cache max is 0");
    rc = chucho_release_writer(wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release remote writer");
}

void run_remote_writer_test(void)
{
    sput_enter_suite("remote_writer");
    sput_run_test(remote_writer_test);
    sput_leave_suite();
}
