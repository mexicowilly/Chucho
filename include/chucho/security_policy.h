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

#if !defined(CHUCHO_SECURITY_POLICY_H__)
#define CHUCHO_SECURITY_POLICY_H__

#include <chucho/return_code.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct chucho_security_policy chucho_security_policy;

CHUCHO_EXPORT chucho_rc chucho_release_security_policy(chucho_security_policy* pol);

CHUCHO_EXPORT chucho_rc chucho_sec_get_default_text_max(const chucho_security_policy* pol, size_t* tmax);
CHUCHO_EXPORT chucho_rc chucho_sec_get_integer_range(const chucho_security_policy* pol,
                                                     const char* const key,
                                                     long long* low,
                                                     long long* high);
CHUCHO_EXPORT chucho_rc chucho_sec_get_text_max(const chucho_security_policy* pol,
                                                const char* const key,
                                                size_t* tmax);
CHUCHO_EXPORT chucho_rc chucho_sec_override_integer(chucho_security_policy* pol,
                                                    const char* const key,
                                                    long long low,
                                                    long long high);
CHUCHO_EXPORT chucho_rc chucho_sec_override_text(chucho_security_policy* pol,
                                                 const char* const key,
                                                 size_t tmax);
CHUCHO_EXPORT chucho_rc chucho_sec_set_default_text_max(chucho_security_policy* pol, size_t tmax);
CHUCHO_EXPORT chucho_rc chucho_sec_set_integer(chucho_security_policy* pol,
                                               const char* const key,
                                               long long low,
                                               long long high);
CHUCHO_EXPORT chucho_rc chucho_sec_set_text(chucho_security_policy* pol,
                                            const char* const key,
                                            size_t tmax);


#if defined(__cplusplus)
}
#endif

#endif
