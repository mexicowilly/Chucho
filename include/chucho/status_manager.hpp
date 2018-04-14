/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_STATUS_MANAGER_HPP__)
#define CHUCHO_STATUS_MANAGER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

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
 * The status_manager by default has a @ref status_observer that 
 * will print any @ref status of @ref status::level "WARNING" or 
 * @ref status::level "ERROR" to std::cout. If, however, any 
 * other observer is added, then this default observer will be 
 * removed. 
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
    static const std::size_t HEAD_CACHE_MAX = 150;
    /**
     * The maximum size of the tail cache. The tail cache contains 
     * the last entries written to the cache. 
     */
    static const std::size_t TAIL_CACHE_MAX = 150;

    /**
     * Return the sole instance of the status_manager.
     * 
     * @return the instance
     */
    static status_manager& get();

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
     * Print the selected status entries to stream. Only entries 
     * with a level greater than or equal to the indicated minimum 
     * level will be printed. Each entry is followed by a platform 
     * appropriate line ending. 
     *  
     * For example, to print only errors and warnings to stdout, the
     * following code could be used. 
     * @code 
     * chucho::status_manager::get().print(std::cout, chucho::status::level::WARNING_);
     * @endcode 
     * 
     * @param stream the stream to which to print the entries
     * @param min_level the minimum status level to print
     */
    void print(std::ostream& stream, status::level min_level);
    /**
     * Remove an observer.
     * 
     * @param obs the observer to remove
     */
    void remove(std::shared_ptr<status_observer> obs);

private:
    friend struct smgr_wrapper;

    status_manager();
    /**
     * @pre guard_ must be locked
     */
    CHUCHO_NO_EXPORT void adjust_level();

    std::vector<status> head_cache_;
    std::deque<status> tail_cache_;
    std::mutex cache_guard_;
    std::size_t count_;
    status::level level_;
    std::set<std::shared_ptr<status_observer>> observers_;
    std::mutex observer_guard_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
