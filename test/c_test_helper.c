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

#include <chucho/configuration.h>
#include <chucho/log.h>
#include <chucho/finalize.h>
#include <stdlib.h>
#include <stdio.h>

const char* fb = \
"- chucho::logger:\n"
"    name: c\n"
"    level: trace\n"
"    chucho::file_writer:\n"
"        file_name: %s\n"
"        on_start: truncate\n"
"        chucho::pattern_formatter:\n"
"            pattern: '%%p%%k %%m%%n'\n";

int main(int argc, char* argv[])
{
    char fallback[1024];
    snprintf(fallback, sizeof(fallback), fb, argv[1]);
    chucho_cnf_set_fallback(fallback);
    chucho_cnf_set_file_name(NULL);
    CHUCHO_C_TRACE("c", "%i", 1);
    CHUCHO_C_TRACE_M("c", " mark", "%i", 2);
    CHUCHO_C_DEBUG("c", "%i", 3);
    CHUCHO_C_DEBUG_M("c", " mark", "%i", 4);
    CHUCHO_C_INFO("c", "%i", 5);
    CHUCHO_C_INFO_M("c", " mark", "%i", 6);
    CHUCHO_C_WARN("c", "%i", 7);
    CHUCHO_C_WARN_M("c", " mark", "%i", 8);
    CHUCHO_C_ERROR("c", "%i", 9);
    CHUCHO_C_ERROR_M("c", " mark", "%i", 10);
    CHUCHO_C_FATAL("c", "%i", 11);
    CHUCHO_C_FATAL_M("c", " mark", "%i", 12);
#if !defined(__SUNPRO_C)
    chucho_finalize();
#endif
    return EXIT_SUCCESS;
}

