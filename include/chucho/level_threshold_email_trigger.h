/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_H__)
#define CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_H__

/**
 * @file level_threshold_email_trigger.h
 * @copydoc chucho::level_threshold_email_trigger
 * 
 * @ingroup c_email
 */

#include <chucho/email_trigger.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @name Creation
 * @{
 */
/**
 * Creat a level threshold email trigger.
 * 
 * @post Ownership of the trg parameter is transferred to the
 *       caller, and trg must be released with the @ref
 *       chucho_release_email_trigger function.
 * 
 * @param[out] trg the email trigger to create
 * @param[in] lvl the level threshold that causes email to be sent
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_level_threshold_email_trigger(chucho_email_trigger** trg,
                                                                    const chucho_level* lvl);
/**
 * @}
 */

/**
 * Return the threshold level of the given level threshold trigger
 * 
 * @param[in] trg the level threshold email trigger
 * @param[out] lvl the threshold level
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_ltetrg_get_level(const chucho_email_trigger* trg, const chucho_level** lvl);

#if defined(__cplusplus)
}
#endif

#endif