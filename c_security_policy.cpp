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

#include <chucho/security_policy.h>
#include <chucho/c_security_policy.hpp>

extern "C"
{

chucho_rc chucho_release_security_policy(chucho_security_policy* pol)
{
    delete pol;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_get_default_text_max(const chucho_security_policy* pol, size_t* tmax)
{
    if (pol == nullptr || tmax == nullptr)
        return CHUCHO_NULL_POINTER;
    *tmax = pol->pol_->get_default_text_max();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_get_integer_range(const chucho_security_policy* pol,
                                       const char* const key,
                                       long long* low,
                                       long long* high)
{
    if (pol == nullptr || key == nullptr || low == nullptr || high == nullptr)
        return CHUCHO_NULL_POINTER;
    auto rng = pol->pol_->get_integer_range(key);
    if (!rng)
        return CHUCHO_NO_SUCH_VALUE;
    *low = rng->first;
    *high = rng->second;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_get_text_max(const chucho_security_policy* pol,
                                  const char* const key,
                                  size_t* tmax)
{
    if (pol == nullptr || key == nullptr || tmax == nullptr)
        return CHUCHO_NULL_POINTER;
    *tmax = pol->pol_->get_text_max(key);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_override_integer(chucho_security_policy* pol,
                                      const char* const key,
                                      long long low,
                                      long long high)
{
    if (pol == nullptr || key == nullptr)
        return CHUCHO_NULL_POINTER;
    pol->pol_->override_integer(key, low, high);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_override_text(chucho_security_policy* pol,
                                   const char* const key,
                                   size_t tmax)
{
    if (pol == nullptr || key == nullptr)
        return CHUCHO_NULL_POINTER;
    pol->pol_->override_text(key, tmax);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_set_default_text_max(chucho_security_policy* pol, size_t tmax)
{
    if (pol == nullptr)
        return CHUCHO_NULL_POINTER;
    pol->pol_->set_default_text_max(tmax);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_set_integer(chucho_security_policy* pol,
                                 const char* const key,
                                 long long low,
                                 long long high)
{
    if (pol == nullptr || key == nullptr)
        return CHUCHO_NULL_POINTER;
    pol->pol_->set_integer(key, low, high);
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_sec_set_text(chucho_security_policy* pol,
                              const char* const key,
                              size_t tmax)
{
    if (pol == nullptr || key == nullptr)
        return CHUCHO_NULL_POINTER;
    pol->pol_->set_text(key, tmax);
    return CHUCHO_NO_ERROR;
}

}
