#include <chucho/logger.hpp>
#include <chucho/logger_memento.hpp>
#include <map>

namespace
{

std::shared_ptr<chucho::logger> root_logger;
std::map<std::string, std::shared_ptr<chucho::logger>> all_loggers;
std::recursive_mutex loggers_guard;
std::once_flag logger_init_once;

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
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
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
    std::lock_guard<std::mutex> lg(writers_guard_);
    writers_.push_back(wrt);
}

std::shared_ptr<logger> logger::get(const std::string& name)
{
    std::call_once(logger_init_once, static_init);
    return get_impl(name);
}

std::shared_ptr<logger> logger::get(const logger_memento& mnto)
{
    std::shared_ptr<logger> lgr(get(mnto.get_name()));
    if (mnto.get_level())
        lgr->set_level(mnto.get_level());
    lgr->writers_ = mnto.get_writers();
    if (mnto.get_writes_to_ancestors())
        lgr->set_writes_to_ancestors(*mnto.get_writes_to_ancestors());
    return lgr;
}

std::shared_ptr<level> logger::get_effective_level() const
{
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    auto lgr = shared_from_this();
    while (!lgr->level_)
        lgr = lgr->parent_;
    return lgr->level_;
}

std::vector<std::shared_ptr<logger>> logger::get_existing_loggers()
{
    std::vector<std::shared_ptr<logger>> result;
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    auto itor = all_loggers.begin();
    while (itor != all_loggers.end())
    {
        if (itor->second.unique())
        {
            all_loggers.erase(itor++);
        }
        else
        {
            result.push_back(itor->second);
            ++itor;
        }
    }
    return result;
}

std::shared_ptr<level> logger::get_level() const
{
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    return level_;
}

std::shared_ptr<logger> logger::get_impl(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    auto found = all_loggers.find(name);
    if (found == all_loggers.end())
    {
        std::shared_ptr<logger> lgr(new logger(name));
        found = all_loggers.emplace(name, lgr).first;
    }
    return found->second;
}

void logger::remove_unused_loggers()
{
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    auto itor = all_loggers.begin();
    while (itor != all_loggers.end())
    {
        if (itor->second.unique())
            all_loggers.erase(itor++);
        else
            ++itor;
    }
}

void logger::set_level(std::shared_ptr<level> lvl)
{
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    level_ = lvl;
}

void logger::static_init()
{
    std::shared_ptr<level> info(new info_level());
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    root_logger.reset(new logger("", info));
    all_loggers[""] = std::shared_ptr<logger>(root_logger);
}

void logger::write(const event& evt)
{
    std::unique_lock<std::mutex> ul(writers_guard_);
    for (std::shared_ptr<writer> w : writers_)
        w->write(evt);
    ul.unlock();
    if (writes_to_ancestors_)
        parent_->write(evt);
}

}
