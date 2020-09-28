/*
 * Copyright 2013-2020 Will Mason
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
 *
 * @ingroup c_loggers
 */

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * A logger. This type can be used with the functions @ref chucho_get_logger
 * and @ref chucho_release_logger in order to make logging slightly more
 * efficient, if and only if your compiler supports the _Generic keyword.
 * In this case, you may use the logging macros with either a logger or
 * the name of a logger as the lgr parameter. If your compiler does not
 * support _Generic, then you may only use the name of the logger in the
 * logging macros, so using this type will be nearly pointless.
 */
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

/**
 * Get a logger.
 *
 * @post The logger must be released with @ref chucho_release_logger
 *
 * @param[in] name the name of the logger
 * @return the newly allocated logger
 */
CHUCHO_EXPORT chucho_logger_t* chucho_get_logger(const char* const name);
/**
 * Release a logger allocated by @ref chucho_get_logger
 *
 * @param[in] lgr the logger to release
 */
CHUCHO_EXPORT void chucho_release_logger(chucho_logger_t* lgr);

/**
 * Test whether a logger permits a given level.
 *
 * @param[in] lgr the logger to test
 * @param[in] lvl the level
 * @return non-zero if the logger permits the level, zero if it does not
 */
CHUCHO_EXPORT int chucho_logger_permits(const chucho_logger_t* lgr, chucho_level_t lvl);
/**
 * Set a logger's level.
 *
 * @param[in] lgr the logger
 * @param[in] lvl the level
 */
CHUCHO_EXPORT void chucho_logger_set_level(chucho_logger_t* lgr, chucho_level_t lvl);

#if defined(__cplusplus)
}
#endif

#endif
