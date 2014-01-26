/*
 * Copyright 2013-2014 Will Mason
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

#if !defined(CHUCHO_REMOTE_WRITER_MEMENTO_HPP__)
#define CHUCHO_REMOTE_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/memento_key_set.hpp>
#include <chucho/optional.hpp>
#include <cstdint>

namespace chucho
{

class remote_writer_memento : public writer_memento
{
public:
    remote_writer_memento(const configurator& cfg, memento_key_set ks);

    const std::string& get_host() const;
    const optional<std::uint16_t>& get_port() const;
    const optional<std::size_t>& get_unsent_cache_max() const;

private:
    std::string host_;
    optional<std::uint16_t> port_;
    optional<std::size_t> unsent_cache_max_;
};

inline const std::string& remote_writer_memento::get_host() const
{
    return host_;
}

inline const optional<std::uint16_t>& remote_writer_memento::get_port() const
{
    return port_;
}

inline const optional<std::size_t>& remote_writer_memento::get_unsent_cache_max() const
{
    return unsent_cache_max_;
}

}

#endif
