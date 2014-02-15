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

#if !defined(CHUCHO_STATUS_MANAGER_H__)
#define CHUCHO_STATUS_MANAGER_H__

/**
 * @file 
 * @copydoc chucho::status_manager 
 *  
 * @ingroup c_status 
 */

#include <chucho/return_code.h>
#include <chucho/status.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * A status observer. You may register an observer with the @ref 
 * chucho_status_add_observer() function. 
 */
typedef void (*chucho_status_observer)(const chucho_status* st);

/**
 * Add a status observer.
 * 
 * @param[in] obs the observer to add
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_status_add_observer(chucho_status_observer obs);
/**
 * Clear all cached status entries.
 * 
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_status_clear(void);
/**
 * Return the number of status events. 
 *  
 * @note This is the number of all reported status events, not 
 *       the number of ones currently held in the cache.
 * 
 * @param[out] cnt the number of status events
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_status_get_count(size_t* cnt);
/**
 * Return the status level.
 * 
 * @param[out] lvl the status level
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_status_get_level(chucho_status_level* lvl);
/**
 * Remove a status observer.
 * 
 * @note It is not an error to remove an observer that is not 
 *       currently registered.
 * 
 * @param obs the observer to remove
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_status_remove_observer(chucho_status_observer obs);

#if defined(__cplusplus)
}
#endif

#endif
