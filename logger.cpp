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

#include <chucho/logger.hpp>
#include <chucho/configurator.hpp>
#include <chucho/configuration.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <map>
#include <stdexcept>
#include <atomic>
#include <algorithm>

namespace
{

struct static_data
{
    static_data();

    std::map<std::string, std::shared_ptr<chucho::logger>> all_loggers_;
    std::recursive_mutex loggers_guard_;
    std::atomic<bool> is_initialized_;
};

static_data::static_data()
    : is_initialized_(false)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
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
    auto ancestors = split(name);
    ancestors.pop_back();
    std::vector<std::shared_ptr<logger>> resolved;
    std::string ancestor_name;
    std::lock_guard<std::recursive_mutex> lg(data().loggers_guard_);
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

void logger::add_writer(std::shared_ptr<writer> wrt)
{
    if (!wrt)
        throw std::invalid_argument("The writer cannot be an uninitialized std::shared_ptr");
    std::lock_guard<std::recursive_mutex> lg(guard_);
    writers_.push_back(wrt);
}

std::shared_ptr<logger> logger::get(const std::string& name)
{
    static_data& sd(data());
    std::lock_guard<std::recursive_mutex> lg(sd.loggers_guard_);
    // std::call_once does not remove the need for the atomic bool,
    // so we just use that and roll our own call_once.
    if (!sd.is_initialized_)
        initialize();
    return get_impl(name);
}

std::shared_ptr<level> logger::get_effective_level() const
{
    std::lock_guard<std::recursive_mutex> lg(data().loggers_guard_);
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
    std::lock_guard<std::recursive_mutex> lg(sd.loggers_guard_);
    for (const std::map<std::string, std::shared_ptr<logger>>::value_type& i : sd.all_loggers_)
        result.push_back(i.second);
    return result;
}

std::shared_ptr<level> logger::get_level()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    return level_;
}

std::shared_ptr<logger> logger::get_impl(const std::string& name)
{
    static_data& sd(data());
    std::lock_guard<std::recursive_mutex> lg(sd.loggers_guard_);
    auto found = sd.all_loggers_.find(name);
    if (found == sd.all_loggers_.end())
    {
        std::shared_ptr<logger> lgr(new logger(name));
        found = sd.all_loggers_.insert(std::make_pair(name, lgr)).first;
    }
    return found->second;
}

std::vector<std::shared_ptr<writer>> logger::get_writers()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    return writers_;
}

void logger::initialize()
{
    static_data& sd(data());
    std::lock_guard<std::recursive_mutex> lg(sd.loggers_guard_);
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
    std::lock_guard<std::recursive_mutex> lg(guard_);
    return *lvl >= *get_effective_level();
}

void logger::remove_all_writers()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    writers_.clear();
}

void logger::remove_unused_loggers()
{
    static_data& sd(data());
    std::lock_guard<std::recursive_mutex> lg(sd.loggers_guard_);
    auto itor = sd.all_loggers_.begin();
    while (itor != sd.all_loggers_.end())
    {
        // The first condition is checking for root. We never
        // want to remove root.
        if (itor->second->parent_ && itor->second.unique())
            sd.all_loggers_.erase(itor++);
        else
            ++itor;
    }
}

void logger::remove_writer(std::shared_ptr<writer> wrt)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    auto found = std::find(writers_.begin(), writers_.end(), wrt);
    if (found != writers_.end())
        writers_.erase(found);
}

void logger::reset()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    writers_.clear();
    level_.reset();
    writes_to_ancestors_ = true;
}

void logger::set_level(std::shared_ptr<level> lvl)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    level_ = lvl;
}

void logger::set_writes_to_ancestors(bool val)
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    writes_to_ancestors_ = val;
}

void logger::write(const event& evt)
{
    std::unique_lock<std::recursive_mutex> ul(guard_);
    for (std::shared_ptr<writer> w : writers_)
        w->write(evt);
    ul.unlock();
    if (parent_ && writes_to_ancestors_)
        parent_->write(evt);
}

bool logger::writes_to_ancestors()
{
    std::lock_guard<std::recursive_mutex> lg(guard_);
    return writes_to_ancestors_;
}

}
