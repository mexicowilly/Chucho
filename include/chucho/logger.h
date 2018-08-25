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

#include <chucho/log.h>

/**
 * @file
 * Chucho's logger functions
 */

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_logger_t chucho_logger_t;

chucho_logger_t* chucho_get_logger(const char* const name);
void chucho_destroy_logger(chucho_logger_t* lgr);

/**
 * Finalize the Chucho library. This function should be called
 * if you believe your memory leak analysis tools when they tell
 * you that static objects that live the life of the application
 * are memory leaks. You should also use this function if you
 * are using Chucho as a shared object or within an enclosing
 * shared object.
 *
 * @note No Chucho APIs may be used after calling this function.
 *       The result is extremely undefined.
 * @ingroup c_miscellaneous
 */
CHUCHO_EXPORT int chucho_logger_permits(const chucho_logger_t* lgr, chucho_level_t lvl);

#if defined(__cplusplus)
}
#endif

#endif
