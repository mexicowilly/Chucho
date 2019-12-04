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

#if !defined(CHUCHO_CACHE_FILTER_MEMENTO_HPP__)
#define CHUCHO_CACHE_FILTER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/filter_memento.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class cache_filter_memento : public filter_memento
{
public:
    cache_filter_memento(configurator& cfg);

    const optional<std::size_t>& get_chunk_size() const;
    const optional<std::size_t>& get_max_chunks() const;
    std::shared_ptr<level> get_threshold() const;

private:
    std::shared_ptr<level> threshold_;
    optional<std::size_t> chunk_size_;
    optional<std::size_t> max_chunks_;
};

inline const optional<std::size_t>& cache_filter_memento::get_chunk_size() const
{
    return chunk_size_;
}

inline const optional<std::size_t>& cache_filter_memento::get_max_chunks() const
{
    return max_chunks_;
}

inline std::shared_ptr<level> cache_filter_memento::get_threshold() const
{
    return threshold_;
}

}

#endif
