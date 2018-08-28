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

#if !defined(CHUCHO_LOGGER_H__)
#define CHUCHO_LOGGER_H__

#if !defined(CHUCHO_BUILD) && defined(__cplusplus)
#error "When using C++, you should include finalize.hpp"
#endif

#include <chucho/export.h>

/**
 * @file
 * Chucho's logger functions
 */

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_logger_t chucho_logger_t;

/**
 * The predefined levels.
 */
typedef enum
{
  CHUCHO_TRACE, /**< Trace */
  CHUCHO_DEBUG, /**< Debug */
  CHUCHO_INFO,  /**< Info */
  CHUCHO_WARN,  /**< Warn */
  CHUCHO_ERROR, /**< Error */
  CHUCHO_FATAL  /**< Fatal */
} chucho_level_t;

CHUCHO_EXPORT chucho_logger_t* chucho_get_logger(const char* const name);
CHUCHO_EXPORT void chucho_destroy_logger(chucho_logger_t* lgr);

/**
 * @ingroup c_miscellaneous
 */
CHUCHO_EXPORT int chucho_logger_permits(const chucho_logger_t* lgr, chucho_level_t lvl);

#if defined(__cplusplus)
}
#endif

#endif
