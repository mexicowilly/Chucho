/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/configuration.h>
#include <chucho/configuration.hpp>

extern "C"
{

bool chucho_cnf_allow_default(void)
{
    return chucho::configuration::allow_default();
}

const char* chucho_cnf_get_environment_variable(void)
{
    const std::string& cvar(chucho::configuration::get_environment_variable());
    return cvar.empty() ? NULL : cvar.c_str();
}

const char* chucho_cnf_get_fallback(void)
{
    const std::string& cfb(chucho::configuration::get_fallback());
    return cfb.empty() ? NULL : cfb.c_str();
}

const char* chucho_cnf_get_file_name(void)
{
    const std::string& cname(chucho::configuration::get_file_name());
    return cname.empty() ? NULL : cname.c_str();
}

void chucho_cnf_set_allow_default(bool val)
{
    chucho::configuration::set_allow_default(val);
}

void chucho_cnf_set_environment_variable(const char* const var)
{
    chucho::configuration::set_environment_variable(var == nullptr ? "" : var);
}

void chucho_cnf_set_fallback(const char* const fb)
{
    chucho::configuration::set_fallback(fb == nullptr ? "" : fb);
}

void chucho_cnf_set_file_name(const char* const name)
{
    chucho::configuration::set_file_name(name == nullptr ? "" : name);
}

}
