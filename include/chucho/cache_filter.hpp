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

#if !defined(CHUCHO_CACHE_FILTER_HPP__)
#define CHUCHO_CACHE_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/event_cache_provider.hpp>
#include <chucho/writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT cache_filter : public filter, public event_cache_provider
{
public:
    cache_filter(const std::string& name,
                 writer& wrt,
                 std::shared_ptr<level> threshold,
                 std::size_t chunk_size,
                 std::size_t max_chunks);
    cache_filter(const std::string& name,
                 const std::string& writer_name,
                 std::shared_ptr<level> threshold,
                 std::size_t chunk_size,
                 std::size_t max_chunks);

    virtual result evaluate(const event& evt) override;

private:
    /* This is just a settable reference */
    writer* writer_;
    std::shared_ptr<level> threshold_;
    std::string writer_name_;
};

}

#endif
