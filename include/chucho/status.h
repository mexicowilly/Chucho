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

#if !defined(CHUCHO_STATUS_H__)
#define CHUCHO_STATUS_H__

/**
 * @file 
 * Types that can be used in status reporting.
 * @copydoc chucho::status
 *  
 * @sa status_manager.h 
 *  
 * @ingroup c_status 
 */

#include <time.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * The level of a status event.
 */
typedef enum
{
    /** 
     * Information only. Nothing is wrong.
     */
    chucho_status_level_info,
    /**
     * Something went wrong, but not badly enough to inhibit 
     * functionality. 
     */
    chucho_status_level_warning,
    /**
     * Chucho is prevented from doing something that it should be 
     * doing. 
     */
    chucho_status_level_error
} chucho_status_level;

/**
 * A status event.
 *  
 * @ingroup c_status 
 */
typedef struct
{
    chucho_status_level level;  /**< The level. */
    const char* message;        /**< The message. */
    const char* origin;         /**< The component of origin. */
    time_t time;                /**< The time. */
} chucho_status;

#if defined(__cplusplus)
}
#endif

#endif
