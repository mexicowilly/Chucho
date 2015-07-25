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
#include "test_util.h"
#include <chucho/logger.h>
#include <chucho/pattern_formatter.h>
#include <chucho/cout_writer.h>
#include <chucho/level.h>

static void add_writer(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "add_writer");
    size_t count;
    chucho_formatter* fmt;
    chucho_writer* wrt;
    chucho_writer* got_wrt;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_writers(lgr, NULL, 0, &count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "get writers");
    sput_fail_unless(count == 0, "no writers");
    rc = chucho_create_pattern_formatter(&fmt, "%m%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_cout_writer(&wrt, fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create cout writer");
    rc = chucho_lgr_add_writer(lgr, wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add writer");
    rc = chucho_lgr_get_writers(lgr, &got_wrt, 1, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get writers");
    sput_fail_unless(count == 1, "one writer");
    rc = chucho_release_writer(got_wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release writer");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
}

static void ancestors(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "ancestors");
    int anc;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_writes_to_ancestors(lgr, &anc);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "writes to ancestors");
    sput_fail_unless(anc == 1, "does write to ancestors");
    rc = chucho_lgr_set_writes_to_ancestors(lgr, 0);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set writes to ancestors");
    rc = chucho_lgr_writes_to_ancestors(lgr, &anc);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "writes to ancestors");
    sput_fail_unless(anc == 0, "does not write to ancestors");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
}

static void levels(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "levels.one");
    const chucho_level* lvl;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_level(lgr, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_SUCH_LEVEL, "get level");
    rc = chucho_lgr_set_level(lgr, chucho_warn_level());
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set level");
    rc = chucho_lgr_get_level(lgr, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level");
    sput_fail_unless(lvl == chucho_warn_level(), "level is WARN");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
    rc = chucho_create_logger(&lgr, "levels.one.two");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_level(lgr, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_SUCH_LEVEL, "get level");
    rc = chucho_lgr_get_effective_level(lgr, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get effective level");
    sput_fail_unless(lvl == chucho_warn_level(), "level is WARN");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
    rc = chucho_remove_unused_loggers();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove unused loggers");
}

static void name(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "name");
    const char* nm;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_name(lgr, &nm);
    sput_fail_unless(strcmp(nm, "name") == 0, "get name");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
    rc = chucho_remove_unused_loggers();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove unused loggers");
}

static void permits(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "permits");
    const chucho_level* lvl;
    int perm;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_level(lgr, &lvl);
    sput_fail_unless(rc == CHUCHO_NO_SUCH_LEVEL, "get level");
    rc = chucho_lgr_set_level(lgr, chucho_error_level());
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "set level");
    rc = chucho_lgr_permits(lgr, chucho_info_level(), &perm);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "permits");
    sput_fail_unless(perm == 0, "does not permit");
    rc = chucho_lgr_permits(lgr, chucho_error_level(), &perm);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "permits");
    sput_fail_unless(perm == 1, "does permit");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
}

static void remove_writers(void)
{
    chucho_logger* lgr;
    chucho_rc rc = chucho_create_logger(&lgr, "remove_writers");
    size_t count;
    chucho_formatter* fmt;
    chucho_writer* wrt;
    chucho_writer* got_wrt;

    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create logger");
    rc = chucho_lgr_get_writers(lgr, NULL, 0, &count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "get writers");
    sput_fail_unless(count == 0, "no writers");
    rc = chucho_create_pattern_formatter(&fmt, "%m%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    rc = chucho_create_cout_writer(&wrt, fmt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create cout writer");
    rc = chucho_lgr_add_writer(lgr, wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add writer");
    rc = chucho_lgr_get_writers(lgr, &got_wrt, 1, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get writers");
    sput_fail_unless(count == 1, "one writer");
    rc = chucho_lgr_remove_writer(lgr, got_wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove writer");
    rc = chucho_lgr_get_writers(lgr, NULL, 0, &count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "get writers");
    sput_fail_unless(count == 0, "no writers");
    rc = chucho_lgr_add_writer(lgr, got_wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add writer");
    rc = chucho_lgr_get_writers(lgr, &got_wrt, 1, &count);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get writers");
    sput_fail_unless(count == 1, "one writer");
    rc = chucho_release_writer(got_wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release writer");
    rc = chucho_lgr_remove_all_writers(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove all writers");
    rc = chucho_lgr_get_writers(lgr, NULL, 0, &count);
    sput_fail_unless(rc == CHUCHO_NULL_POINTER, "get writers");
    sput_fail_unless(count == 0, "no writers");
    rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger");
}

void run_logger_test(void)
{
    sput_enter_suite("logger");
    sput_run_test(add_writer);
    sput_run_test(ancestors);
    sput_run_test(levels);
    sput_run_test(name);
    sput_run_test(permits);
    sput_run_test(remove_writers);
    sput_leave_suite();
}
