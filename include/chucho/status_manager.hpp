#if !defined(CHUCHO_STATUS_MANAGER_HPP__)
#define CHUCHO_STATUS_MANAGER_HPP__

#include <chucho/status_observer.hpp>
#include <vector>
#include <mutex>
#include <deque>
#include <set>

namespace chucho
{

class CHUCHO_EXPORT status_manager
{
public:
    const std::size_t FIXED_CACHE_MAX = 150;
    const std::size_t TAILED_CACHE_MAX = 150;

    static std::shared_ptr<status_manager> get();

    status_manager();

    void add(const status& st);
    void add(std::shared_ptr<status_observer> obs);
    void clear();
    std::vector<status> get_all();
    std::size_t get_count();
    status::level get_level();
    void remove(std::shared_ptr<status_observer> obs);

private:
    /**
     * @pre guard_ must be locked
     */
    void adjust_level();

    std::vector<status> fixed_cache_;
    std::deque<status> tailed_cache_;
    std::mutex cache_guard_;
    std::size_t count_;
    status::level level_;
    std::set<std::shared_ptr<status_observer>, std::owner_less<std::shared_ptr<status_observer>>> observers_;
    std::mutex observer_guard_;
};

}

#endif
