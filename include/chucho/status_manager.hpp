#if !defined(CHUCHO_STATUS_MANAGER_HPP__)
#define CHUCHO_STATUS_MANAGER_HPP__

#include <chucho/status.hpp>
#include <vector>
#include <mutex>
#include <deque>

namespace chucho
{

class CHUCHO_EXPORT status_manager
{
public:
    const std::size_t FIXED_CACHE_MAX = 150;
    const std::size_t TAILED_CACHE_MAX = 150;

    status_manager();

    void add(const status& st);
    void clear();
    std::vector<status> get_all();
    std::size_t get_count();
    status::level get_level();

private:
    std::vector<status> fixed_cache_;
    std::deque<status> tailed_cache_;
    std::mutex cache_guard_;
    std::size_t count_;
    status::level level_;
};

}

#endif
