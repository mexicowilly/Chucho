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

#if !defined(CHUCHO_EMAIL_TRIGGER_H__)
#define CHUCHO_EMAIL_TRIGGER_H__

/**
 * @file email_trigger.h
 * @copydoc chucho::email_trigger
 * 
 * @ingroup c_email
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque structure that you don't care about.
 */
typedef struct chucho_email_trigger chucho_email_trigger;

/**
 * Release an email trigger.
 * 
 * @post Ownership of the trg parameter is transferred to the
 *       callee. The trg object may not be used after calling
 *       this function.
 * 
 * @param[in] trg the email trigger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_email_trigger(chucho_email_trigger* trg);

#if defined(__cplusplus)
}
#endif

#endif
