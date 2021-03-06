/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/logger.hpp>
#include <chucho/configurator.hpp>
#include <chucho/configuration.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/time_util.hpp>
#include <chucho/demangle.hpp>
#include <chucho/regex.hpp>
#include <map>
#include <stdexcept>
#include <atomic>
#include <algorithm>
#include <sstream>

namespace
{

struct static_data
{
    static_data();

    std::map<std::string, std::shared_ptr<chucho::logger>> all_loggers_;
    std::mutex loggers_guard_;
    std::atomic<bool> is_initialized_;
};

static_data::static_data()
    : is_initialized_(false)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

static_data& data()
{
    static std::once_flag once;
    // This will be cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

std::vector<std::string> split(const std::string& name)
{
    std::vector<std::string> result;
    std::string::size_type pos1 = 0;
    std::string::size_type pos2;
    do
    {
        pos2 = name.find('.', pos1);
        if (pos2 > pos1)
            result.emplace_back(name.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
    } while (pos2 != std::string::npos);
    return result;
}

}

namespace chucho
{

logger::logger(const std::string& name, std::shared_ptr<level> lvl)
    : name_(name),
      level_(lvl),
      writes_to_ancestors_(true)
{
    set_status_origin("logger");
    auto ancestors = split(name);
    ancestors.pop_back();
    std::vector<std::shared_ptr<logger>> resolved;
    std::string ancestor_name;
    static_data& sd(data());
    for (std::string& a : ancestors)
    {
        if (!ancestor_name.empty())
            ancestor_name += '.';
        ancestor_name += a;
        resolved.push_back(get_impl(ancestor_name));
    }
    if (resolved.empty())
    {
        if (!name.empty())
            parent_ = get_impl("");
    }
    else
    {
        if (!resolved[0]->parent_)
            resolved[0]->parent_ = get_impl("");
        for (std::size_t i = 1; i < resolved.size(); i++)
        {
            if (!resolved[i]->parent_)
                resolved[i]->parent_ = resolved[i - 1];
        }
        parent_ = resolved.back();
    }
}

logger::~logger()
{
    for (auto& w : writers_)
    {
        try
        {
            w->flush();
        }
        catch (std::exception& e)
        {
            report_error(name_ + " error flushing " + demangle::get_demangled_name(typeid(*w)) + ": " + e.what());
        }
    }
}

void logger::add_writer(std::unique_ptr<writer>&& wrt)
{
    if (!wrt)
        throw std::invalid_argument("The writer cannot be an uninitialized std::unique_ptr");
    std::lock_guard<std::mutex> lg(guard_);
    writers_.push_back(std::move(wrt));
}

std::shared_ptr<logger> logger::get(const std::string& name)
{
    static_data& sd(data());
    std::lock_guard<std::mutex> lg(sd.loggers_guard_);
    // std::call_once does not remove the need for the atomic bool,
    // so we just use that and roll our own call_once.
    if (!sd.is_initialized_)
        initialize();
    return get_impl(name);
}

std::shared_ptr<level> logger::get_effective_level() const
{
    std::lock_guard<std::mutex> lg(data().loggers_guard_);
    auto lgr = shared_from_this();
    while (!lgr->level_ && lgr->parent_)
        lgr = lgr->parent_;
    // Some idiot could have set no level on the root logger
    return lgr->level_ ? lgr->level_ : level::OFF_();
}

std::vector<std::shared_ptr<logger>> logger::get_existing_loggers()
{
    std::vector<std::shared_ptr<logger>> result;
    static_data& sd(data());
    std::lock_guard<std::mutex> lg(sd.loggers_guard_);
    for (const std::map<std::string, std::shared_ptr<logger>>::value_type& i : sd.all_loggers_)
        result.push_back(i.second);
    return result;
}

std::shared_ptr<logger> logger::get_impl(const std::string& name)
{
    static_data& sd(data());
    auto found = sd.all_loggers_.find(name);
    if (found == sd.all_loggers_.end())
    {
        std::shared_ptr<logger> lgr(new logger(name));
        found = sd.all_loggers_.insert(std::make_pair(name, lgr)).first;
    }
    return found->second;
}

std::shared_ptr<level> logger::get_level()
{
    std::lock_guard<std::mutex> lg(guard_);
    return level_;
}

writer& logger::get_writer(const std::string& name)
{
    std::lock_guard<std::mutex> lg(guard_);
    auto found = std::find_if(writers_.begin(),
                              writers_.end(),
                              [&name](const std::unique_ptr<writer>& w) { return w->get_name() == name; });
    if (found == writers_.end())
        throw std::invalid_argument("Writer '" + name + "' was not found");
    return **found;
}

std::vector<std::string> logger::get_writer_names()
{
    std::vector<std::string> result;
    std::lock_guard<std::mutex> lg(guard_);
    for (const auto& w : writers_)
        result.push_back(w->get_name());
    return result;
}

void logger::initialize()
{
    time_util::start_now();
    static_data& sd(data());
    // When loggers are created during configuration, this variable
    // must be true, so that we don't recurse the initialization.
    sd.is_initialized_ = true;
    std::shared_ptr<logger> root(new logger("", level::INFO_()));
    sd.all_loggers_[root->get_name()] = root;
    if (configuration::get_style() == configuration::style::AUTOMATIC)
        configuration::perform(root);
}

bool logger::permits(std::shared_ptr<level> lvl)
{
    std::lock_guard<std::mutex> lg(guard_);
    return *lvl >= *get_effective_level();
}

void logger::clear_writers()
{
    std::lock_guard<std::mutex> lg(guard_);
    writers_.clear();
}

void logger::remove_unused_loggers()
{
    class reporter : public status_reporter
    {
    public:
        ~reporter()
        {
            try
            {
                std::ostringstream stream;
                stream << "Removed " << names_.size() << " unused logger";
                std::string msg;
                if (names_.empty())
                {
                    stream << 's';
                    msg = stream.str();
                }
                else
                {
                    if (names_.size() > 1)
                        stream << 's';
                    stream << ':';
                    for (const auto& name : names_)
                        stream << " '" << name << "',";
                    msg = stream.str();
                    msg.pop_back();
                }
                report_info(msg);
            }
            catch (...)
            {
                // Failure to report this message is not important enough to get upset
            }
        }

        void add(const std::string& to_add)
        {
            names_.push_back(to_add);
        }

    private:
        std::vector<std::string> names_;
    };

    static_data& sd(data());
    std::lock_guard<std::mutex> lg(sd.loggers_guard_);
    std::set<std::string> to_erase;
    for (const auto& cur : sd.all_loggers_)
    {
        // Never erase root
        if (cur.second->parent_)
        {
            // If it's all alone, then this one's gone.
            if (cur.second.unique())
                to_erase.insert(cur.first);
            // If the child is the only logger that holds a
            // reference to the parent and the parent is not
            // root, then the parent is gone.
            // (use_count 2 because 1 is the child and the other is all_loggers_)
            if (cur.second->parent_.use_count() == 2 &&
                cur.second->parent_->parent_)
            {
                to_erase.insert(cur.second->parent_->get_name());
            }
        }
    }
    reporter rpt;
    for (const auto& tgt : to_erase)
    {
        sd.all_loggers_.erase(tgt);
        rpt.add(tgt);
    }
}

void logger::remove_writer(const std::string& wrt)
{
    std::lock_guard<std::mutex> lg(guard_);
    writers_.remove_if([&wrt] (const std::unique_ptr<writer>& w) { return w->get_name() == wrt; });
}

void logger::reset()
{
    std::lock_guard<std::mutex> lg(guard_);
    writers_.clear();
    level_.reset();
    writes_to_ancestors_ = true;
}

void logger::set_level(std::shared_ptr<level> lvl)
{
    std::lock_guard<std::mutex> lg(guard_);
    level_ = lvl;
}

void logger::set_writes_to_ancestors(bool val)
{
    std::lock_guard<std::mutex> lg(guard_);
    writes_to_ancestors_ = val;
}

std::string logger::type_to_logger_name(const std::type_info& info)
{
    static regex::expression re(".anonymous namespace.");

    auto name = demangle::get_demangled_name(info);
    name = regex::replace(name, re, "~");
    auto found = name.find("::");
    while (found != std::string::npos)
    {
        name.replace(found, 2, 1, '.');
        found = name.find("::", found + 1);
    }
    return name;
}

void logger::write(const event& evt)
{
    std::unique_lock<std::mutex> ul(guard_);
    for (auto& w : writers_)
        w->write(evt);
    ul.unlock();
    if (parent_ && writes_to_ancestors_)
        parent_->write(evt);
}

bool logger::writes_to_ancestors()
{
    std::lock_guard<std::mutex> lg(guard_);
    return writes_to_ancestors_;
}

}
