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

#if !defined(CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_H__)
#define CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_H__

/**
 * @file 
 * @copydoc chucho::interval_file_roll_trigger 
 *  
 * @ingroup c_rolling 
 */

#include <chucho/file_roll_trigger.h>
#include <sys/types.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * A period of time. Interval file roll triggers are constructed 
 * with an interval period and a count. 
 */
typedef enum
{
    CHUCHO_INTERVAL_PERIOD_MINUTE,  /**< Minute */
    CHUCHO_INTERVAL_PERIOD_HOUR,    /**< Hour */
    CHUCHO_INTERVAL_PERIOD_DAY,     /**< Day */
    CHUCHO_INTERVAL_PERIOD_WEEK,    /**< Week */
    CHUCHO_INTERVAL_PERIOD_MONTH    /**< Month */
} chucho_interval_period;

/**
 * @name Creation
 * @{
 */
/**
 * Create an interval file roll trigger.
 * 
 * @post Ownership of the trg parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_file_roll_trigger() function.
 * 
 * @param[out] trg the interval file roll trigger to create
 * @param[in] prd the period of time consistuting the interval
 * @param[in] count the number of periods to wait before 
 *       triggering
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_interval_file_roll_trigger(chucho_file_roll_trigger** trg,
                                                                 chucho_interval_period prd,
                                                                 size_t count);
/** @} */

#if defined(__cplusplus)
}
#endif

#endif
