#include <chucho/status_manager.hpp>

namespace chucho
{

status_manager::status_manager()
    : count_(0),
      level_(status::level::INFO)
{
}

void status_manager::add(const status& st)
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    count_++;
    if (st.get_level() > level_)
        level_ = st.get_level();
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
}

void status_manager::clear()
{
    std::lock_guard<std::mutex> lg(cache_guard_);
    count_ = 0;
    fixed_cache_.clear();
    tailed_cache_.clear();
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

}
