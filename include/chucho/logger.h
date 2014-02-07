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

#if !defined(CHUCHO_LOGGER_H__)
#define CHUCHO_LOGGER_H__

#include <chucho/level.h>
#include <chucho/writer.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_logger chucho_logger;

CHUCHO_EXPORT chucho_rc chucho_create_logger(chucho_logger** lgr, const char* const name);
CHUCHO_EXPORT chucho_rc chucho_release_logger(chucho_logger* lgr);
CHUCHO_EXPORT chucho_rc chucho_remove_unused_loggers(void);

CHUCHO_EXPORT chucho_rc chucho_lgr_add_writer(chucho_logger* lgr, chucho_writer* wrt);
CHUCHO_EXPORT chucho_rc chucho_lgr_get_effective_level(const chucho_logger* lgr, const chucho_level** lvl);
CHUCHO_EXPORT chucho_rc chucho_lgr_get_level(const chucho_logger* lgr, const chucho_level** lvl);
CHUCHO_EXPORT chucho_rc chucho_lgr_get_writers(const chucho_logger* lgr, chucho_writer** buf, size_t buf_size, size_t* count);
CHUCHO_EXPORT chucho_rc chucho_lgr_permits(const chucho_logger* lgr, const chucho_level* lvl, int* state);
CHUCHO_EXPORT chucho_rc chucho_lgr_remove_all_writers(chucho_logger* lgr);
CHUCHO_EXPORT chucho_rc chucho_lgr_remove_writer(chucho_logger* lgr, chucho_writer* wrt);
CHUCHO_EXPORT chucho_rc chucho_lgr_reset(chucho_logger* lgr);
CHUCHO_EXPORT chucho_rc chucho_lgr_set_level(chucho_logger* lgr, const chucho_level* lvl);
CHUCHO_EXPORT chucho_rc chucho_lgr_set_writes_to_ancestors(chucho_logger* lgr, int state);
CHUCHO_EXPORT chucho_rc chucho_lgr_writes_to_ancestors(const chucho_logger* lgr, int* state);

#if defined(__cplusplus)
}
#endif

#endif
