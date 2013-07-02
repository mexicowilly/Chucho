/*
 * Copyright 2013 Will Mason
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
    return chucho::syslog::severity::DEBUG;
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
    return chucho::syslog::severity::DEBUG;
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
    class shared_level_name_less
    {
    public:
        shared_level_name_less(levels& l);

        bool operator() (std::shared_ptr<chucho::level> lvl_one,
                         std::shared_ptr<chucho::level> lvl_two) const;

    private:
        levels& lvls_;
    };

    levels();

    std::string to_upper(const std::string& text);
    
    std::shared_ptr<chucho::level> TRACE;
    std::shared_ptr<chucho::level> DEBUG;
    std::shared_ptr<chucho::level> INFO;
    std::shared_ptr<chucho::level> WARN;
    std::shared_ptr<chucho::level> ERROR;
    std::shared_ptr<chucho::level> FATAL;
    std::shared_ptr<chucho::level> OFF;
    std::mutex guard_;
    std::locale c_loc_;
    std::set<std::shared_ptr<chucho::level>, shared_level_name_less> all_levels_;
};

levels::levels()
    : TRACE(new trace()),
      DEBUG(new debug()),
      INFO(new info()),
      WARN(new warn()),
      ERROR(new error()),
      FATAL(new fatal()),
      OFF(new off()),
      c_loc_("C"),
      all_levels_(shared_level_name_less(*this))
{
    all_levels_.insert(TRACE);
    all_levels_.insert(DEBUG);
    all_levels_.insert(INFO);
    all_levels_.insert(WARN);
    all_levels_.insert(ERROR);
    all_levels_.insert(FATAL);
    all_levels_.insert(OFF);
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::string levels::to_upper(const std::string& text)
{
    std::string up;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(up),
                   [this] (char c) { return std::toupper(c, c_loc_); });
    return up;
}

levels::shared_level_name_less::shared_level_name_less(levels& l)
    : lvls_(l)
{
}

bool levels::shared_level_name_less::operator() (std::shared_ptr<chucho::level> lvl_one,
                                                 std::shared_ptr<chucho::level> lvl_two) const
{
    return lvls_.to_upper(lvl_one->get_name()) < lvls_.to_upper(lvl_two->get_name());
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

std::shared_ptr<level> level::TRACE()
{
    return lvls().TRACE;
}

std::shared_ptr<level> level::DEBUG()
{
    return lvls().DEBUG;
}

std::shared_ptr<level> level::INFO()
{
    return lvls().INFO;
}

std::shared_ptr<level> level::WARN()
{
    return lvls().WARN;
}

std::shared_ptr<level> level::ERROR()
{
    return lvls().ERROR;
}

std::shared_ptr<level> level::FATAL()
{
    return lvls().FATAL;
}

std::shared_ptr<level> level::OFF()
{
    return lvls().OFF;
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
    std::string up = l.to_upper(text);
    auto found = std::find_if(l.all_levels_.begin(),
                              l.all_levels_.end(),
                              [&] (std::shared_ptr<level> lvl) { return l.to_upper(lvl->get_name()) == up; });
    if (found == l.all_levels_.end())
        throw exception("The text " + text + " does not describe a valid log level");
    return *found;
}

}
