#if !defined(CHUCHO_STATUS_MANAGER_HPP__)
#define CHUCHO_STATUS_MANAGER_HPP__

#include <chucho/status_observer.hpp>
#include <vector>
#include <mutex>
#include <deque>
#include <set>

namespace chucho
{

/**
 * @class status_manager status_manager.hpp chucho/status_manager.hpp 
 * Manage all status reporting. As chucho does its job it 
 * reports status to the status_manager. It keeps a limited 
 * cache of status messages, which can be retrieved for 
 * examination. 
 * 
 * @ingroup status
 */
class CHUCHO_EXPORT status_manager
{
public:
    /**
     * The maximum size of the head cache. The head cache contains 
     * the first entries written to the cache. 
     */
    const std::size_t HEAD_CACHE_MAX = 150;
    /**
     * The maximum size of the tail cache. The tail cache contains 
     * the last entries written to the cache. 
     */
    const std::size_t TAIL_CACHE_MAX = 150;

    /**
     * Return the sole instance of the status_manager.
     * 
     * @return the instance
     */
    static std::shared_ptr<status_manager> get();

    /**
     * Add a status entry.
     * 
     * @param st the status entry
     */
    void add(const status& st);
    /**
     * Add a @ref status_observer. Status observers can be used to 
     * get a view of the status in real time. 
     * 
     * @param obs the observer
     */
    void add(std::shared_ptr<status_observer> obs);
    /**
     * Remove all cached status entries.
     */
    void clear();
    /**
     * Return all cached status entries. The cache contains the 
     * first 150 entries and the last 150 entries. Therefore, there 
     * may be a gap in the returned collection where some entries 
     * were deleted. 
     * 
     * @return the status cache
     */
    std::vector<status> get_all();
    /**
     * Return the number of status entries.
     * 
     * @note The number returned is the number of all reported 
     *       status entries, not the number currently held in the
     *       cache.
     * @return the total number of reported status entries
     */
    std::size_t get_count();
    /**
     * Return the status level. This method can be used to determine 
     * whether any warnings or errors have occurred without having 
     * to examine the cache. 
     * 
     * @return the status level
     */
    status::level get_level();
    /**
     * Remove an observer.
     * 
     * @param obs the observer to remove
     */
    void remove(std::shared_ptr<status_observer> obs);

private:
    status_manager();

    /**
     * @pre guard_ must be locked
     */
    void adjust_level();

    std::vector<status> head_cache_;
    std::deque<status> tail_cache_;
    std::mutex cache_guard_;
    std::size_t count_;
    status::level level_;
    std::set<std::shared_ptr<status_observer>> observers_;
    std::mutex observer_guard_;
};

}

#endif
