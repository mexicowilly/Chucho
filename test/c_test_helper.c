/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/configuration.h>
#include <chucho/log.h>
#include <chucho/finalize.h>
#include <stdlib.h>
#include <stdio.h>

const char* fb = \
"- chucho::logger:\n"
"    name: <root>\n"
"    level: trace\n"
"    chucho::file_writer:\n"
"        file_name: %s\n"
"        on_start: truncate\n"
"        chucho::pattern_formatter:\n"
"            pattern: '%%p%%k %%c %%m%%n'\n";

int main(int argc, char* argv[])
{
    char fallback[1024];
    chucho_logger_t* lgr;

    snprintf(fallback, sizeof(fallback), fb, argv[1]);
    chucho_cnf_set_fallback(fallback);
    chucho_cnf_set_file_name(NULL);
    lgr = chucho_get_logger("c_by_logger");
    CHUCHO_C_TRACE("c", "%i", 1);
    CHUCHO_C_TRACE(lgr, "%i", 2);
    CHUCHO_C_TRACE_M("c", " mark", "%i", 3);
    CHUCHO_C_TRACE_M(lgr, " mark", "%i", 4);
    CHUCHO_C_DEBUG("c", "%i", 5);
    CHUCHO_C_DEBUG(lgr, "%i", 6);
    CHUCHO_C_DEBUG_M("c", " mark", "%i", 7);
    CHUCHO_C_DEBUG_M(lgr, " mark", "%i", 8);
    CHUCHO_C_INFO("c", "%i", 9);
    CHUCHO_C_INFO(lgr, "%i", 10);
    CHUCHO_C_INFO_M("c", " mark", "%i", 11);
    CHUCHO_C_INFO_M(lgr, " mark", "%i", 12);
    CHUCHO_C_WARN("c", "%i", 13);
    CHUCHO_C_WARN(lgr, "%i", 14);
    CHUCHO_C_WARN_M("c", " mark", "%i", 15);
    CHUCHO_C_WARN_M(lgr, " mark", "%i", 16);
    CHUCHO_C_ERROR("c", "%i", 17);
    CHUCHO_C_ERROR(lgr, "%i", 18);
    CHUCHO_C_ERROR_M("c", " mark", "%i", 19);
    CHUCHO_C_ERROR_M(lgr, " mark", "%i", 20);
    CHUCHO_C_FATAL("c", "%i", 21);
    CHUCHO_C_FATAL(lgr, "%i", 22);
    CHUCHO_C_FATAL_M("c", " mark", "%i", 23);
    CHUCHO_C_FATAL_M(lgr, " mark", "%i", 24);
    chucho_release_logger(lgr);
#if !defined(__SUNPRO_C)
    chucho_finalize();
#endif
    return EXIT_SUCCESS;
}

