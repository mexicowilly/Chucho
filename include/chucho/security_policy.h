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

#if !defined(CHUCHO_SECURITY_POLICY_H__)
#define CHUCHO_SECURITY_POLICY_H__

/**
 * @file 
 * Functions for managing the security policy.
 * @copydoc chucho::security_policy
 *  
 * @ingroup c_miscellaneous
 */

#include <chucho/return_code.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_security_policy chucho_security_policy;

/**
 * Release a security policy. Note that any changes made to the 
 * security policy are preserved in the internal guts of Chucho.
 *  
 * @post Ownership of the pol parameter is transferred to the 
 *       callee.
 * 
 * @param[in] pol the security policy to release
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_security_policy(chucho_security_policy* pol);
/**
 * Return the maximum length of text by default for keys that do 
 * not have explicit settings. 
 * 
 * @param[in] pol the security policy
 * @param[out] tmax the default maximum text length
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_get_default_text_max(const chucho_security_policy* pol, size_t* tmax);
/**
 * Return the allowed range of values for integers associated 
 * with the given key. 
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[out] low the lowest allowed value
 * @param[out] high the highest allowed value
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_get_integer_range(const chucho_security_policy* pol,
                                                     const char* const key,
                                                     long long* low,
                                                     long long* high);
/**
 * Return the maximum length of pieces of text associated with 
 * the given key. 
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[out] tmax the maximum length for the key, or the 
 *       default length if no explicit setting has been made for
 *       the key
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_get_text_max(const chucho_security_policy* pol,
                                                const char* const key,
                                                size_t* tmax);
/**
 * Set an integer range. This function clobbers any existing 
 * setting for the given key. The function @ref 
 * chucho_sec_set_integer will not replace an existing value. 
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[in] low the lowest value allowed
 * @param[in] high the highest value allowed
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_override_integer(chucho_security_policy* pol,
                                                    const char* const key,
                                                    long long low,
                                                    long long high);
/**
 * Set a maximum length for text associated with the given key. 
 * This function clobbers any existing setting for the given 
 * key. The function @ref chucho_sec_set_text will not replace 
 * an existing value. 
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[in] tmax the maximum text length
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_override_text(chucho_security_policy* pol,
                                                 const char* const key,
                                                 size_t tmax);
/**
 * Set the default maximum length of text. If no specific 
 * maximum text length is set for the key, then the default is 
 * used. 
 * 
 * @param[in] pol the security policy 
 * @param[in] tmax the maximum length of text by default 
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_set_default_text_max(chucho_security_policy* pol, size_t tmax);
/**
 * Set an integer range. This function will not replace an 
 * existing range for the key. If you wish to unconditionally 
 * set the range, then please use @ref 
 * chucho_sec_override_integer. 
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[in] low the minimum value of the range
 * @param[in] high the maximum value of the range
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_sec_set_integer(chucho_security_policy* pol,
                                               const char* const key,
                                               long long low,
                                               long long high);
/**
 * Set a text maximum length. This function will not replace an 
 * existing maximum length for the key. If you wish to 
 * unconditionally set the maximum length, then please use 
 * @ref chucho_sec_override_text.
 * 
 * @param[in] pol the security policy
 * @param[in] key the key
 * @param[in] tmax the maximum value of the length
 */
CHUCHO_EXPORT chucho_rc chucho_sec_set_text(chucho_security_policy* pol,
                                            const char* const key,
                                            size_t tmax);


#if defined(__cplusplus)
}
#endif

#endif
