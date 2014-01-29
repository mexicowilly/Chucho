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
#include "test_util.h"
#include <chucho/logger.h>
#include <chucho/file_writer.h>
#include <chucho/pattern_formatter.h>
#include <chucho/error.h>
#include <chucho/log.h>

#define LOG_FUNCTION(lvl) \
    static void log_##lvl(const char* const mark)                       \
    {                                                                   \
        sput_enter_suite_fixture("log "#lvl , set_up, tear_down);       \
        sput_run_test(set_log_level);                                   \
        int rc = chucho_lgr_set_level(lgr, chucho_##lvl##_level());     \
        sput_fail_unless(rc == CHUCHO_NO_ERROR, "set logger level");    \
        static_mark = mark;                                             \
        if (mark == NULL)                                               \
            sput_run_test(log);                                         \
        else                                                            \
            sput_run_test(log_mark);                                    \
        expect(chucho_##lvl##_level(), mark);                           \
        sput_leave_suite();                                             \
    }



static chucho_logger* lgr;
static const char* static_mark;

static void log()
{
    CHUCHO_C_TRACE(lgr, "my dog %s", "has fleas");
    CHUCHO_C_DEBUG(lgr, "my dog %s", "has fleas");
    CHUCHO_C_INFO(lgr, "my dog %s", "has fleas");
    CHUCHO_C_WARN(lgr, "my dog %s", "has fleas");
    CHUCHO_C_ERROR(lgr, "my dog %s", "has fleas");
    CHUCHO_C_FATAL(lgr, "my dog %s", "has fleas");
}

static void log_mark()
{
    CHUCHO_C_TRACE_M(lgr, static_mark, "my dog %s", "has fleas");
    CHUCHO_C_DEBUG_M(lgr, static_mark, "my dog %s", "has fleas");
    CHUCHO_C_INFO_M(lgr, static_mark, "my dog %s", "has fleas");
    CHUCHO_C_WARN_M(lgr, static_mark, "my dog %s", "has fleas");
    CHUCHO_C_ERROR_M(lgr, static_mark, "my dog %s", "has fleas");
    CHUCHO_C_FATAL_M(lgr, static_mark, "my dog %s", "has fleas");
}

static char** read_lines()
{
    FILE* f = fopen("log_macro.log", "r");
    if (f == NULL) 
        return NULL;
    char** result = calloc(10, sizeof(char*));
    if (result == NULL) 
    {
        fclose(f);
        return NULL;
    }
    for (int i = 0; i < 9; i++) 
    {
        result[i] = read_line(f);
        if (result[i] == NULL) 
            break;
    }
    fclose(f);
    return result;
}

static void expect(const chucho_level* lvl, const char* mark)
{
    char** lines = read_lines();
    sput_fail_if(lines == NULL, "read file");
    if (lines == NULL) 
        return;
    const char* nm;
    int rc = chucho_lvl_get_name(lvl, &nm);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level name");
    if (strcmp(nm, "OFF") == 0) 
    {
        sput_fail_unless(lines[0] == NULL, "got no lines");
        return;
    }
    else
    {
        sput_fail_if(lines[0] == NULL, "got lines");
        if (lines[0] == NULL)
            return;
    }
    const chucho_level* lvls[] =
    {
        chucho_trace_level(),
        chucho_debug_level(),
        chucho_info_level(),
        chucho_warn_level(),
        chucho_error_level(),
        chucho_fatal_level()
    };
    int tgt_val;
    rc = chucho_lvl_get_value(lvl, &tgt_val);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level value");
    int lvl_idx = 0;
    int line_idx = 0;
    int val;
    while (lvl_idx < 6 && lines[line_idx] != NULL)
    {
        rc = chucho_lvl_get_value(lvls[lvl_idx], &val);
        sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level value");
        if (val >= tgt_val) 
        {
            rc = chucho_lvl_get_name(lvls[lvl_idx], &nm);
            sput_fail_unless(rc == CHUCHO_NO_ERROR, "get level name");
            sput_fail_unless(strstr(lines[line_idx], nm) == lines[line_idx], "level match");
            if (mark != NULL) 
                sput_fail_if(strstr(lines[line_idx], mark) == NULL, "find mark");
            free(lines[line_idx++]);
        }
        lvl_idx++;
    }
    free(lines);
}

static void set_log_level()
{
    // This function does nothing but help set the test name in sput
}

static void set_up(void)
{
    int rc = chucho_create_logger(&lgr, "log_macro");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "get logger log_macro");
    chucho_formatter* fmt;
    rc = chucho_create_pattern_formatter(&fmt, "%p %m %k%n");
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create pattern formatter");
    chucho_writer* wrt;
    rc = chucho_create_file_writer(&wrt, fmt, "log_macro.log", CHUCHO_ON_START_TRUNCATE, 1);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "create file writer");
    rc = chucho_lgr_add_writer(lgr, wrt);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "add writer");
}

static void tear_down(void)
{
    int rc = chucho_release_logger(lgr);
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "release logger log_macro");
    rc = chucho_remove_unused_loggers();
    sput_fail_unless(rc == CHUCHO_NO_ERROR, "remove unused loggers");
    sput_fail_unless(remove("log_macro.log") == 0, "remove log_macro.log");
}

LOG_FUNCTION(trace)
LOG_FUNCTION(debug)
LOG_FUNCTION(info)
LOG_FUNCTION(warn)
LOG_FUNCTION(error)
LOG_FUNCTION(fatal)
LOG_FUNCTION(off)

void run_log_macro_test(void)
{
    log_trace(NULL);
    log_trace("mark");
    log_debug(NULL);
    log_debug("mark");
    log_info(NULL);
    log_info("mark");
    log_warn(NULL);
    log_warn("mark");
    log_error(NULL);
    log_error("mark");
    log_fatal(NULL);
    log_fatal("mark");
    log_off(NULL);
    log_off("mark");
}
