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

#if !defined(CHUCHO_CONFIGURATION_H__)
#define CHUCHO_CONFIGURATION_H__

#include <chucho/export.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef enum
{
    CHUCHO_CONFIGURATION_STYLE_AUTOMATIC,
    CHUCHO_CONFIGURATION_STYLE_OFF
} chucho_configuration_style;

typedef int (*chucho_unknown_handler_type)(const char* const key, const char* const val);

CHUCHO_EXPORT int chucho_cnf_allow_default(int* val);
CHUCHO_EXPORT int chucho_cnf_get_environment_variable(const char** var);
CHUCHO_EXPORT int chucho_cnf_get_fallback(const char** fb);
CHUCHO_EXPORT int chucho_cnf_get_file_name(const char** name);
CHUCHO_EXPORT int chucho_cnf_get_loaded_file_name(const char** name);
CHUCHO_EXPORT int chucho_cnf_get_max_size(size_t* sz);
CHUCHO_EXPORT int chucho_cnf_get_style(chucho_configuration_style* sty);
CHUCHO_EXPORT int chucho_cnf_get_unknown_handler(chucho_unknown_handler_type* func);
CHUCHO_EXPORT int chucho_cnf_reconfigure(int* rc);
CHUCHO_EXPORT int chucho_cnf_set_allow_default(int val);
CHUCHO_EXPORT int chucho_cnf_set_environment_variable(const char* const var);
CHUCHO_EXPORT int chucho_cnf_set_fallback(const char* const fb);
CHUCHO_EXPORT int chucho_cnf_set_file_name(const char* const name);
CHUCHO_EXPORT int chucho_cnf_set_max_size(size_t sz);
CHUCHO_EXPORT int chucho_cnf_set_style(chucho_configuration_style sty);
CHUCHO_EXPORT int chucho_cnf_set_unknown_handler(chucho_unknown_handler_type func);

#if defined(__cplusplus)
}
#endif

#endif
