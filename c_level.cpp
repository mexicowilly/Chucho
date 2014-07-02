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
#include <chucho/text_util.hpp>
#include <map>
#include <thread>
#include <limits>
#include <assert.h>

namespace
{

class c_level : public chucho::level
{
public:
    c_level(const char* const name, int value, chucho_syslog_severity sev);

    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;

private:
    std::string name_;
    int value_;
    chucho::syslog::severity severity_;
};

class chucho_level_less
{
public:
    bool operator() (const std::string& one, const std::string& two) const
    {
        return chucho::text_util::to_lower(one) < chucho::text_util::to_lower(two);
    }
};

struct static_data
{
    static_data();

    std::map<std::string, std::shared_ptr<chucho_level>, chucho_level_less> levels_;
};

c_level::c_level(const char* const name, int value, chucho_syslog_severity sev)
    : name_(name),
      value_(value),
      severity_(static_cast<chucho::syslog::severity>(sev))
{
}

const char* c_level::get_name() const
{
    return name_.c_str();
}

chucho::syslog::severity c_level::get_syslog_severity() const
{
    return severity_;
}

int c_level::get_value() const
{
    return value_;
}

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

chucho_rc chucho_add_level(const chucho_level** lvl,
                           const char* const name,
                           int value,
                           chucho_syslog_severity sev)
{
    if (lvl == nullptr || name == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        auto cpp = std::make_shared<c_level>(name, value, sev);
        bool state = chucho::level::add(cpp);
        if (state) 
        {
            auto c = std::make_shared<chucho_level>();
            c->level_ = cpp;
            data().levels_[name] = c;
            *lvl = c.get();
        }
        else
        {
            return CHUCHO_LEVEL_ALREADY_EXISTS;
        }
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_get_level(const chucho_level** lvl, const char* const name)
{
    try
    {
        static_data& sd(data());
        auto found = sd.levels_.find(name);
        if (found == sd.levels_.end()) 
            return CHUCHO_NO_SUCH_LEVEL;
        *lvl = found->second.get();
    }
    catch (...)
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

chucho_rc chucho_lvl_get_name(const chucho_level* lvl, const char** name)
{
    if (lvl == nullptr || name == nullptr) 
        return CHUCHO_NULL_POINTER;
    *name = lvl->level_->get_name();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lvl_get_syslog_severity(const chucho_level* lvl, chucho_syslog_severity* sev)
{
    if (lvl == nullptr || sev == nullptr)
        return CHUCHO_NULL_POINTER;
    *sev = static_cast<chucho_syslog_severity>(lvl->level_->get_syslog_severity());
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_lvl_get_value(const chucho_level* lvl, int* val)
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
