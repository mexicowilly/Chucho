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

#include <chucho/level.hpp>
#include <chucho/exception.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/text_util.hpp>
#include <limits>
#include <set>
#include <algorithm>
#include <mutex>

namespace
{

class trace : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class debug : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class info : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class warn : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class error : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class fatal : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

class off : public chucho::level
{
public:
    virtual const char* get_name() const override;
    virtual chucho::syslog::severity get_syslog_severity() const override;
    virtual int get_value() const override;
};

const char* trace::get_name() const
{
    return "TRACE";
}

chucho::syslog::severity trace::get_syslog_severity() const
{
    return chucho::syslog::severity::DEBUG_;
}

int trace::get_value() const
{
    return 0;
}

const char* debug::get_name() const
{
    return "DEBUG";
}

chucho::syslog::severity debug::get_syslog_severity() const
{
    return chucho::syslog::severity::DEBUG_;
}

int debug::get_value() const
{
    return 10000;
}

const char* info::get_name() const
{
    return "INFO";
}

chucho::syslog::severity info::get_syslog_severity() const
{
    return chucho::syslog::severity::INFORMATIONAL;
}

int info::get_value() const
{
    return 20000;
}

chucho::syslog::severity warn::get_syslog_severity() const
{
    return chucho::syslog::severity::WARNING;
}

const char* warn::get_name() const
{
    return "WARN";
}

int warn::get_value() const
{
    return 30000;
}

const char* error::get_name() const
{
    return "ERROR";
}

chucho::syslog::severity error::get_syslog_severity() const
{
    return chucho::syslog::severity::ERROR;
}

int error::get_value() const
{
    return 40000;
}

const char* fatal::get_name() const
{
    return "FATAL";
}

chucho::syslog::severity fatal::get_syslog_severity() const
{
    return chucho::syslog::severity::EMERGENCY;
}

int fatal::get_value() const
{
    return 50000;
}

const char* off::get_name() const
{
    return "OFF";
}

chucho::syslog::severity off::get_syslog_severity() const
{
    return chucho::syslog::severity::EMERGENCY;
}

int off::get_value() const
{
    return std::numeric_limits<int>::max();
}

struct levels
{
    class level_name_less
    {
    public:
        bool operator() (std::shared_ptr<chucho::level> lvl_one,
                         std::shared_ptr<chucho::level> lvl_two) const;
    };

    levels();

    std::shared_ptr<chucho::level> TRACE_;
    std::shared_ptr<chucho::level> DEBUG_;
    std::shared_ptr<chucho::level> INFO_;
    std::shared_ptr<chucho::level> WARN_;
    std::shared_ptr<chucho::level> ERROR_;
    std::shared_ptr<chucho::level> FATAL_;
    std::shared_ptr<chucho::level> OFF_;
    std::mutex guard_;
    std::set<std::shared_ptr<chucho::level>, level_name_less> all_levels_;
};

levels::levels()
    : TRACE_(new trace()),
      DEBUG_(new debug()),
      INFO_(new info()),
      WARN_(new warn()),
      ERROR_(new error()),
      FATAL_(new fatal()),
      OFF_(new off())
{
    all_levels_.insert(TRACE_);
    all_levels_.insert(DEBUG_);
    all_levels_.insert(INFO_);
    all_levels_.insert(WARN_);
    all_levels_.insert(ERROR_);
    all_levels_.insert(FATAL_);
    all_levels_.insert(OFF_);
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

bool levels::level_name_less::operator() (std::shared_ptr<chucho::level> lvl_one,
                                          std::shared_ptr<chucho::level> lvl_two) const
{
    return chucho::text_util::to_lower(lvl_one->get_name()) <
           chucho::text_util::to_lower(lvl_two->get_name());
}

std::once_flag once;

levels& lvls()
{
    // This gets cleaned in finalize()
    static levels* ls;

    std::call_once(once, [&] () { ls = new levels(); });
    return *ls;
}

}

namespace chucho
{

std::shared_ptr<level> level::TRACE_()
{
    return lvls().TRACE_;
}

std::shared_ptr<level> level::DEBUG_()
{
    return lvls().DEBUG_;
}

std::shared_ptr<level> level::INFO_()
{
    return lvls().INFO_;
}

std::shared_ptr<level> level::WARN_()
{
    return lvls().WARN_;
}

std::shared_ptr<level> level::ERROR_()
{
    return lvls().ERROR_;
}

std::shared_ptr<level> level::FATAL_()
{
    return lvls().FATAL_;
}

std::shared_ptr<level> level::OFF_()
{
    return lvls().OFF_;
}

std::ostream& operator<< (std::ostream& stream, const level& lvl)
{
    stream << lvl.get_name();
    return stream;
}

level::~level()
{
}

bool level::add(std::shared_ptr<level> lvl)
{
    levels& l(lvls());
    std::lock_guard<std::mutex> guard(l.guard_);
    return l.all_levels_.insert(lvl).second;
}

std::shared_ptr<level> level::from_text(const std::string& text)
{
    levels& l(lvls());
    std::lock_guard<std::mutex> guard(l.guard_);
    std::string down = text_util::to_lower(text);
    auto found = std::find_if(l.all_levels_.begin(),
                              l.all_levels_.end(),
                              [&] (std::shared_ptr<level> lvl) { return text_util::to_lower(lvl->get_name()) == down; });
    if (found == l.all_levels_.end())
        throw exception("The text " + text + " does not describe a valid log level");
    return *found;
}

}
