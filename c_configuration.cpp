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

#include <chucho/configuration.h>
#include <chucho/configuration.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/c_security_policy.hpp>
#include <mutex>

namespace
{

struct static_data
{
    static_data();

    chucho_unknown_handler_type handler_;
};

static_data::static_data()
    : handler_(nullptr)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

static_data& data()
{
    static std::once_flag once;
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

void c_unknown_handler(const std::string& key, const std::string& val)
{
    if (data().handler_ != nullptr) 
        (*data().handler_)(key.c_str(), val.c_str());
}

}

extern "C"
{

chucho_rc chucho_cnf_allow_default(int* val)
{
    if (val == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *val = chucho::configuration::allow_default() ? 1 : 0;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_environment_variable(const char** var)
{
    if (var == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        const std::string& cvar(chucho::configuration::get_environment_variable());
        *var = cvar.empty() ? NULL : cvar.c_str();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_fallback(const char** fb)
{
    if (fb == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        const std::string& cfb(chucho::configuration::get_fallback());
        *fb = cfb.empty() ? NULL : cfb.c_str();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_file_name(const char** name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        const std::string& cname(chucho::configuration::get_file_name());
        *name = cname.empty() ? NULL : cname.c_str();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_loaded_file_name(const char** name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        const std::string& cname(chucho::configuration::get_loaded_file_name());
        *name = cname.empty() ? NULL : cname.c_str();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_max_size(size_t* sz)
{
    if (sz == nullptr)
        return CHUCHO_NULL_POINTER; 
    try
    {
        *sz = chucho::configuration::get_max_size();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_security_policy(chucho_security_policy** pol)
{
    if (pol == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *pol = new chucho_security_policy;
        (*pol)->pol_ = &chucho::configuration::get_security_policy();
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_style(chucho_configuration_style* sty)
{
    if (sty == nullptr)
        return CHUCHO_NULL_POINTER; 
    try
    {
        *sty = chucho::configuration::get_style() == chucho::configuration::style::AUTOMATIC ?
            CHUCHO_CONFIGURATION_STYLE_AUTOMATIC : CHUCHO_CONFIGURATION_STYLE_OFF;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_get_unknown_handler(chucho_unknown_handler_type* func)
{
    if (func == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *func = data().handler_;
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_reconfigure(int* rc)
{
    if (rc == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        *rc = chucho::configuration::reconfigure() ? 1 : 0;
    }
    catch (...) 
    {
        // Only bad_alloc will possibly slip out
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_set_allow_default(int val)
{
    try
    {
        chucho::configuration::set_allow_default(val == 0 ? false : true);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_set_environment_variable(const char* const var)
{
    if (var == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho::configuration::set_environment_variable(var == nullptr ? "" : var);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR; 
}

chucho_rc chucho_cnf_set_fallback(const char* const fb)
{
    if (fb == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho::configuration::set_fallback(fb == nullptr ? "" : fb);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR; 
}

chucho_rc chucho_cnf_set_file_name(const char* const name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    try
    {
        chucho::configuration::set_file_name(name == nullptr ? "" : name);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR; 
}

chucho_rc chucho_cnf_set_max_size(size_t sz)
{
    try
    {
        chucho::configuration::set_max_size(sz);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_set_style(chucho_configuration_style sty)
{
    try
    {
        chucho::configuration::set_style(sty == CHUCHO_CONFIGURATION_STYLE_AUTOMATIC ?
            chucho::configuration::style::AUTOMATIC : chucho::configuration::style::OFF);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_cnf_set_unknown_handler(chucho_unknown_handler_type func)
{
    try
    {
        data().handler_ = func;
        chucho::configuration::set_unknown_handler(c_unknown_handler);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

}
