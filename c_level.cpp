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

#include <chucho/level.h>
#include <chucho/c_level.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/error.h>
#include <map>
#include <thread>
#include <limits>
#include <assert.h>

namespace
{

struct static_data
{
    static_data();

    std::map<std::string, std::shared_ptr<chucho_level>> levels_;
};

static_data::static_data()
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
    auto lvl = std::make_shared<chucho_level>();
    lvl->level_ = chucho::level::TRACE_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::DEBUG_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::INFO_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::WARN_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::ERROR_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::FATAL_();
    levels_[lvl->level_->get_name()] = lvl;
    lvl.reset(new chucho_level());
    lvl->level_ = chucho::level::OFF_();
    levels_[lvl->level_->get_name()] = lvl;
}

std::once_flag once;

static_data& data()
{
    // This will be cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

extern "C"
{

const chucho_level* chucho_debug_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "DEBUG");
    assert(lvl != nullptr);
    return lvl;
}

const chucho_level* chucho_error_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "ERROR");
    assert(lvl != nullptr);
    return lvl;
}

const chucho_level* chucho_fatal_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "FATAL");
    assert(lvl != nullptr);
    return lvl;
}

int chucho_get_level(const chucho_level** lvl, const char* const name)
{
    try
    {
        static_data& sd(data());
        auto found = sd.levels_.find(name);
        if (found == sd.levels_.end()) 
        {
            *lvl = nullptr;
            return CHUCHO_NO_SUCH_LEVEL;
        }
        *lvl = found->second.get();
    }
    catch (std::bad_alloc&) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

const chucho_level* chucho_info_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "INFO");
    assert(lvl != nullptr);
    return lvl;
}

int chucho_lvl_get_name(const chucho_level* lvl, const char** name)
{
    if (lvl == nullptr || name == nullptr) 
        return CHUCHO_NULL_POINTER;
    *name = lvl->level_->get_name();
    return CHUCHO_NO_ERROR;
}

int chucho_lvl_get_syslog_severity(const chucho_level* lvl, chucho_syslog_severity* sev)
{
    if (lvl == nullptr || sev == nullptr)
        return CHUCHO_NULL_POINTER;
    *sev = static_cast<chucho_syslog_severity>(lvl->level_->get_syslog_severity());
    return CHUCHO_NO_ERROR;
}

int chucho_lvl_get_value(const chucho_level* lvl, int* val)
{
    if (lvl == nullptr || val == nullptr) 
        return CHUCHO_NULL_POINTER;
    *val = lvl->level_->get_value();
    return CHUCHO_NO_ERROR;
}

const chucho_level* chucho_off_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "OFF");
    assert(lvl != nullptr);
    return lvl;
}

const chucho_level* chucho_trace_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "TRACE");
    assert(lvl != nullptr);
    return lvl;
}

const chucho_level* chucho_warn_level(void)
{
    const chucho_level* lvl;
    chucho_get_level(&lvl, "WARN");
    assert(lvl != nullptr);
    return lvl;
}

}
