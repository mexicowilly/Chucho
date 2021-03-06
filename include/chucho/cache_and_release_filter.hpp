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

#if !defined(CHUCHO_CACHE_AND_RELEASE_FILTER_HPP_)
#define CHUCHO_CACHE_AND_RELEASE_FILTER_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writeable_filter.hpp>
#include <chucho/event_cache_provider.hpp>

namespace chucho
{

/**
 * @class cache_and_release_filter cache_and_release_filter.hpp chucho/cache_and_release_filter.hpp
 * A filter for caching messages below a certain level threshold and then
 * releasing them when a higher level threshold is reached. For example,
 * if you set a cache threshold of of DEBUG, then all messages at DEBUG
 * level or below will be filtered from immediate output, but also placed
 * in a cache. If you then set the release threshold to ERROR, then when
 * an ERROR level message is received, all messages in the cache will be
 * logged before the ERROR message itself. Note that in this scenario,
 * messages higher than DEBUG, like those of INFO level, will not be
 * filtered.
 *
 * @ingroup filters
 */
class CHUCHO_EXPORT cache_and_release_filter : public writeable_filter, public event_cache_provider
{
public:
    /**
     * @name Constructors
     * @{
     */
    /**
     * Construct a filter.
     *
     * @param name the name of the filter
     * @param wrt the writer
     * @param cache_threshold the cache threshold
     * @param release_threshold the release threshold
     * @param chunk_size the size of the chunks of the event cache
     * @param max_chunks the maximum number of chunks allowed
     */
    cache_and_release_filter(const std::string& name,
                             writer& wrt,
                             std::shared_ptr<level> cache_threshold,
                             std::shared_ptr<level> release_threshold,
                             std::size_t chunk_size = DEFAULT_CHUNK_SIZE,
                             std::size_t max_chunks = DEFAULT_MAX_CHUNKS);
    /**
     * Construct a filter.
     *
     * @post This filter will not become usable until the @ref set_writer method
     * is called.
     *
     * @param name the name of the filter
     * @param cache_threshold the cache threshold
     * @param release_threshold the release threshold
     * @param chunk_size the size of the chunks of the event cache
     * @param max_chunks the maximum number of chunks allowed
     */
    cache_and_release_filter(const std::string& name,
                             std::shared_ptr<level> cache_threshold,
                             std::shared_ptr<level> release_threshold,
                             std::size_t chunk_size = DEFAULT_CHUNK_SIZE,
                             std::size_t max_chunks = DEFAULT_MAX_CHUNKS);
    /**
     * @}
     */

    /**
     * Evaluate an event. Return DENY if the level of the event is at or below
     * the cache threshold. The event is also cached in this case. Return
     * NEUTRAL if the level is about the cache threshold and below the release
     * threshold. Return ACCEPT and flush the cache if the level is at or above
     * the release threshold.
     *
     * @param evt the event to check
     * @return the result
     */
    virtual result evaluate(const event& evt) override;
    /**
     * Return the cache thresold.
     * @return the cache threshold
     */
    std::shared_ptr<level> get_cache_threshold() const;
    /**
     * Return the release thresold.
     * @return the release threshold
     */
    std::shared_ptr<level> get_release_threshold() const;

private:
    std::shared_ptr<level> release_threshold_;
    std::shared_ptr<level> cache_threshold_;
};

inline std::shared_ptr<level> cache_and_release_filter::get_cache_threshold() const
{
    return cache_threshold_;
}

inline std::shared_ptr<level> cache_and_release_filter::get_release_threshold() const
{
    return release_threshold_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
