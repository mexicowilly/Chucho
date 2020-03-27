/*
 * Copyright 2013-2019 Will Mason
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

#if !defined(CHUCHO_CACHE_AND_RELEASE_FILTER_HPP__)
#define CHUCHO_CACHE_AND_RELEASE_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/event_cache_provider.hpp>
#include <chucho/writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT cache_and_release_filter : public filter, public event_cache_provider
{
public:
    cache_and_release_filter(const std::string& name,
                             writer& wrt,
                             std::shared_ptr<level> cache_threshold,
                             std::shared_ptr<level> release_threshold,
                             std::size_t chunk_size = DEFAULT_CHUNK_SIZE,
                             std::size_t max_chunks = DEFAULT_MAX_CHUNKS);
    cache_and_release_filter(const std::string& name,
                             std::shared_ptr<level> cache_threshold,
                             std::shared_ptr<level> release_threshold,
                             std::size_t chunk_size = DEFAULT_CHUNK_SIZE,
                             std::size_t max_chunks = DEFAULT_MAX_CHUNKS);

    virtual result evaluate(const event& evt) override;
    std::shared_ptr<level> get_cache_threshold() const;
    std::shared_ptr<level> get_release_threshold() const;
    writer& get_writer() const;
    void set_writer(writer& wrt);

private:
    /* This is just a settable reference */
    writer* writer_;
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

inline writer& cache_and_release_filter::get_writer() const
{
    return *writer_;
}

inline void cache_and_release_filter::set_writer(writer& wrt)
{
    writer_ = &wrt;
}

}

#endif
