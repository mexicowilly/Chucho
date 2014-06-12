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

#if !defined(CHUCHO_FILE_ROLL_TRIGGER_H__)
#define CHUCHO_FILE_ROLL_TRIGGER_H__

/**
 * @file 
 * Types and functions common to all file roll triggers. 
 * @copydoc chucho::file_roll_trigger
 *  
 * @ingroup c_rolling 
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_file_roll_trigger chucho_file_roll_trigger;

/**
 * Release a file roll trigger that was created by one of the 
 * trigger creation functions. 
 * 
 * @post Ownership of the trg parameter is transferred to the 
 *       callee.
 * 
 * @param trg the file roll trigger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_file_roll_trigger(chucho_file_roll_trigger* trg);

#if defined(__cplusplus)
}
#endif

#endif
