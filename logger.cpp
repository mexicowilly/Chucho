#include <chucho/logger.hpp>
#include <map>

namespace
{

std::shared_ptr<chucho::logger> root_logger;
std::map<std::string, std::weak_ptr<chucho::logger>> all_loggers;
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
      level_(lvl)
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
        resolved.push_back(get_logger_impl(ancestor_name));
    }
    if (resolved.empty())
    {
        if (!name.empty())
            parent_ = get_logger_impl("");
    }
    else
    {
        if (!resolved[0]->parent_)
            resolved[0]->parent_ = get_logger_impl("");
        for (std::size_t i = 1; i < resolved.size(); i++)
        {
            if (!resolved[i]->parent_)
                resolved[i]->parent_ = resolved[i - 1];
        }
        parent_ = resolved.back();
    }
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
        if (itor->second.expired())
        {
            all_loggers.erase(itor++);
        }
        else
        {
            result.push_back(itor->second.lock());
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

std::shared_ptr<logger> logger::get_logger(const std::string& name)
{
    std::call_once(logger_init_once, static_init);
    return get_logger_impl(name);
}

std::shared_ptr<logger> logger::get_logger_impl(const std::string& name)
{
    std::shared_ptr<logger> result;
    std::lock_guard<std::recursive_mutex> lg(loggers_guard);
    auto found = all_loggers.find(name);
    if (found == all_loggers.end() || found->second.expired())
    {
        if (found != all_loggers.end())
            all_loggers.erase(found);
        result.reset(new logger(name));
        found = all_loggers.emplace(name, std::weak_ptr<logger>(result)).first;
    }
    else
    {
        result = found->second.lock();
    }
    return result;
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
    all_loggers[""] = std::weak_ptr<logger>(root_logger);
}

}
