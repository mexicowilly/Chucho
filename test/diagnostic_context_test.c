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
#include <chucho/diagnostic_context.h>
#include <chucho/error.h>
#include <string.h>

static void at(void)
{
    int rc = chucho_dgc_set("at-1", "at-one");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set");
    const char* one = chucho_dgc_at("at-1");
    sput_fail_if(one == NULL, "one not NULL");
    sput_fail_unless(strcmp(one, "at-one") == 0, "at-1 is at-one");
    one = chucho_dgc_at("huh");
    sput_fail_if(one == NULL, "one not NULL");
    sput_fail_unless(strcmp(one, "") == 0, "one is empty");
}

static void clear_empty(void)
{
    int rc = chucho_dgc_clear();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "clear");
    rc = chucho_dgc_empty();
    sput_fail_unless(rc == 1, "empty");
    rc = chucho_dgc_set("clear-1", "clear-one");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set");
    rc = chucho_dgc_empty();
    sput_fail_unless(rc == 0, "not empty");
    rc = chucho_dgc_clear();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "clear");
    rc = chucho_dgc_empty();
    sput_fail_unless(rc == 1, "empty");
}

static void erase(void)
{
    int rc = chucho_dgc_clear();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "clear");
    rc = chucho_dgc_set("erase-1", "erase-one");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set erase-1");
    const char* one = chucho_dgc_at("erase-1");
    sput_fail_if(one == NULL, "erase-1 not NULL");
    sput_fail_unless(strcmp(one, "erase-one") == 0, "erase-1 is erase-one");
    rc = chucho_dgc_erase("erase-1");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "erase erase-1");
    rc = chucho_dgc_empty();
    sput_fail_unless(rc == 1, "empty");
}

static void get(void)
{
    int rc = chucho_dgc_clear();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "clear");
    const char* keys[] = { "1", "2", "3" };
    const char* values[] = { "one", "two", "three" };
    int i;
    for (i = 0; i < 3; i++) 
    {
        rc = chucho_dgc_set(keys[i], values[i]);
        sput_fail_unless(rc == CHUCHO_NO_ERROR, "set");
    }
    chucho_dgc_node* nodes[3];
    size_t count;
    rc = chucho_dgc_get(nodes, 3, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get");
    sput_fail_unless(count == 3, "node count of 3");
    int got_1 = 0;
    int got_2 = 0;
    int got_3 = 0;
    for (i = 0; i < 3; i++)
    {
        sput_fail_if(nodes[i] == NULL, "non-NULL node");
        if (nodes[i] != NULL) 
        {
            if (strcmp(nodes[i]->key, "1") == 0) 
            {
                sput_fail_unless(strcmp(nodes[i]->value, "one") == 0, "1 is one");
                got_1 = 1;
            }
            else if (strcmp(nodes[i]->key, "2") == 0) 
            {
                sput_fail_unless(strcmp(nodes[i]->value, "two") == 0, "2 is two");
                got_2 = 1;
            }
            else if (strcmp(nodes[i]->key, "3") == 0) 
            {
                sput_fail_unless(strcmp(nodes[i]->value, "three") == 0, "3 is three");
                got_3 = 1;
            }
            else
            {
                sput_fail_if(1 == 1, "unexpected node");
            }
        }
    }
    sput_fail_unless(got_1 == 1, "got 1");
    sput_fail_unless(got_2 == 1, "got 2");
    sput_fail_unless(got_3 == 1, "got 3");
    for (int i = 0; i < 3; i++) 
    {
        rc = chucho_dgc_release_node(nodes[i]);
        sput_fail_unless(rc == CHUCHO_NO_ERROR, "release nodes");
    }
}

void run_diagnostic_context_test(void)
{
    sput_enter_suite("diagnostic_context");
    sput_run_test(at);
    sput_run_test(clear_empty);
    sput_run_test(erase);
    sput_run_test(get);
    sput_leave_suite();
}
