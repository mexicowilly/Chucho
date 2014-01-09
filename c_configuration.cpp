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

#include <chucho/configuration.h>
#include <chucho/configuration.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/error.h>
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

std::once_flag once;

static_data& data()
{
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

bool c_unknown_handler(const std::string& key, const std::string& val)
{
    if (data().handler_ != nullptr) 
        return (*data().handler_)(key.c_str(), val.c_str()) == 0 ? false : true;
    return false;
}

}

extern "C"
{

int chucho_cnf_allow_default(int* val)
{
    if (val == nullptr)
        return CHUCHO_NULL_POINTER;
    *val = chucho::configuration::allow_default() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_environment_variable(const char** var)
{
    if (var == nullptr)
        return CHUCHO_NULL_POINTER;
    const std::string& cvar(chucho::configuration::get_environment_variable());
    *var = cvar.empty() ? NULL : cvar.c_str();
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_fallback(const char** fb)
{
    if (fb == nullptr)
        return CHUCHO_NULL_POINTER;
    const std::string& cfb(chucho::configuration::get_fallback());
    *fb = cfb.empty() ? NULL : cfb.c_str();
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_file_name(const char** name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    const std::string& cname(chucho::configuration::get_file_name());
    *name = cname.empty() ? NULL : cname.c_str();
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_loaded_file_name(const char** name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    const std::string& cname(chucho::configuration::get_loaded_file_name());
    *name = cname.empty() ? NULL : cname.c_str();
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_max_size(size_t* sz)
{
    if (sz == nullptr)
        return CHUCHO_NULL_POINTER; 
    *sz = chucho::configuration::get_max_size();
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_style(chucho_configuration_style* sty)
{
    if (sty == nullptr)
        return CHUCHO_NULL_POINTER; 
    *sty = chucho::configuration::get_style() == chucho::configuration::style::AUTOMATIC ?
        CHUCHO_CONFIGURATION_STYLE_AUTOMATIC : CHUCHO_CONFIGURATION_STYLE_OFF;
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_get_unknown_handler(chucho_unknown_handler_type* func)
{
    if (func == nullptr)
        return CHUCHO_NULL_POINTER;
    *func = data().handler_;
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_reconfigure(int* rc)
{
    if (rc == nullptr)
        return CHUCHO_NULL_POINTER;
    *rc = chucho::configuration::reconfigure() ? 1 : 0;
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_set_allow_default(int val)
{
    chucho::configuration::set_allow_default(val == 0 ? false : true);
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_set_environment_variable(const char* const var)
{
    if (var == nullptr)
        return CHUCHO_NULL_POINTER;
    chucho::configuration::set_environment_variable(var);
    return CHUCHO_NO_ERROR; 
}

int chucho_cnf_set_fallback(const char* const fb)
{
    if (fb == nullptr)
        return CHUCHO_NULL_POINTER;
    chucho::configuration::set_fallback(fb);
    return CHUCHO_NO_ERROR; 
}

int chucho_cnf_set_file_name(const char* const name)
{
    if (name == nullptr)
        return CHUCHO_NULL_POINTER;
    chucho::configuration::set_file_name(name);
    return CHUCHO_NO_ERROR; 
}

int chucho_cnf_set_max_size(size_t sz)
{
    chucho::configuration::set_max_size(sz);
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_set_style(chucho_configuration_style sty)
{
    chucho::configuration::set_style(sty == CHUCHO_CONFIGURATION_STYLE_AUTOMATIC ?
        chucho::configuration::style::AUTOMATIC : chucho::configuration::style::OFF);
    return CHUCHO_NO_ERROR;
}

int chucho_cnf_set_unknown_handler(chucho_unknown_handler_type func)
{
    data().handler_ = func;
    chucho::configuration::set_unknown_handler(c_unknown_handler);
    return CHUCHO_NO_ERROR;
}

}
