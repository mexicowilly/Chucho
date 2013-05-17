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
#include <limits>
#include <set>
#include <algorithm>

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

std::string to_upper(const std::string& text)
{
    static std::locale c_loc("C");

    std::string up;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(up),
                   [&] (char c) { return std::toupper(c, c_loc); });
    return up;
}

bool shared_level_name_less(std::shared_ptr<chucho::level> l1,
                            std::shared_ptr<chucho::level> l2)
{
    std::string up1 = to_upper(l1->get_name());
    std::string up2 = to_upper(l2->get_name());
    return up1 < up2;
}

std::once_flag lvls_once;
std::mutex lvls_guard;
std::set<std::shared_ptr<chucho::level>, std::function<bool(std::shared_ptr<chucho::level>, std::shared_ptr<chucho::level>)>>
    lvls(shared_level_name_less);

void initialize_lvls()
{
    lvls.insert(chucho::level::TRACE);
    lvls.insert(chucho::level::DEBUG);
    lvls.insert(chucho::level::INFO);
    lvls.insert(chucho::level::WARN);
    lvls.insert(chucho::level::ERROR);
    lvls.insert(chucho::level::FATAL);
    lvls.insert(chucho::level::OFF);
}

}

namespace chucho
{

std::shared_ptr<level> level::TRACE(new trace());
std::shared_ptr<level> level::DEBUG(new debug());
std::shared_ptr<level> level::INFO(new info());
std::shared_ptr<level> level::WARN(new warn());
std::shared_ptr<level> level::ERROR(new error());
std::shared_ptr<level> level::FATAL(new fatal());
std::shared_ptr<level> level::OFF(new off());

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
    std::lock_guard<std::mutex> guard(lvls_guard);
    std::call_once(lvls_once, initialize_lvls);
    return lvls.insert(lvl).second;
}

std::shared_ptr<level> level::from_text(const std::string& text)
{
    std::lock_guard<std::mutex> guard(lvls_guard);
    std::call_once(lvls_once, initialize_lvls);
    std::string up = to_upper(text);
    auto found = std::find_if(lvls.begin(),
                              lvls.end(),
                              [&] (std::shared_ptr<level> lvl) { return to_upper(lvl->get_name()) == up; });
    if (found == lvls.end())
        throw exception("The text " + text + " does not describe a valid log level");
    return *found;
}

}
