#include <chucho/status_manager.hpp>

namespace
{

struct level_less
{
    bool operator() (const chucho::status& st1, const chucho::status& st2) const
    {
        return st1.get_level() < st2.get_level();
    }
};

}

namespace chucho
{

status_manager::status_manager()
    : count_(0),
      level_(status::level::INFO)
{
}

void status_manager::add(const status& st)
{
    std::unique_lock<std::mutex> ul(cache_guard_);
    count_++;
    if (fixed_cache_.size() < FIXED_CACHE_MAX)
    {
        fixed_cache_.push_back(st);
    }
    else
    {
        tailed_cache_.push_back(st);
        if (tailed_cache_.size() > TAILED_CACHE_MAX)
            tailed_cache_.pop_front();
    }
    adjust_level();
    ul.unlock();
    ul = std::unique_lock<std::mutex>(observer_guard_);
    for (std::shared_ptr<status_observer> obs : observers_)
        obs->status_reported(st);
}

void status_manager::add(std::shared_ptr<status_observer> obs)
{
    std::lock_guard<std::mutex> lg(observer_guard_);
    observers_.insert(obs);
}

void status_manager::adjust_level()
{
    status::level fixed_max = fixed_cache_.empty() ?
        status::level::INFO :
        (*std::max_element(fixed_cache_.begin(),
                           fixed_cache_.end(),
                           level_less())).get_level();
    status::level tailed_max = tailed_cache_.empty() ?
        status::level::INFO :
        (*std::max_element(tailed_cache_.begin(),
                           tailed_cache_.end(),
                           level_less())).get_level();
    level_ = std::max(fixed_max, tailed_max);
}

void status_manager::clear()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    count_ = 0;
    fixed_cache_.clear();
    tailed_cache_.clear();
    level_ = status::level::INFO;
}

std::shared_ptr<status_manager> status_manager::get()
{
    static std::shared_ptr<chucho::status_manager> smgr(std::make_shared<chucho::status_manager>());

    return smgr;
}

std::vector<status> status_manager::get_all()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    std::vector<status> result(fixed_cache_);
    result.insert(result.end(), tailed_cache_.begin(), tailed_cache_.end());
    return result;
}

std::size_t status_manager::get_count()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    return count_;
}

status::level status_manager::get_level()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    return level_;
}

void status_manager::remove(std::shared_ptr<status_observer> obs)
{
    std::lock_guard<std::mutex> lg(observer_guard_);
    observers_.erase(obs);
}

}
