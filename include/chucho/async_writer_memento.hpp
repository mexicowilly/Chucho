/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_ASYNC_WRITER_MEMENTO_HPP__)
#define CHUCHO_ASYNC_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>
#include <chucho/writer.hpp>

namespace chucho
{

class async_writer_memento : public memento
{
public:
    async_writer_memento(configurator& cfg);

    const optional<std::size_t>& get_chunk_size() const;
    const optional<bool>& get_flush_on_destruct() const;
    const optional<std::size_t>& get_max_chunks() const;
    const std::string& get_name() const;
    std::unique_ptr<writer>& get_writer();
    virtual void handle(std::unique_ptr<configurable>&& cnf) override;

private:
    optional<std::size_t> chunk_size_;
    optional<std::size_t> max_chunks_;
    std::unique_ptr<writer> writer_;
    optional<bool> flush_on_destruct_;
    std::string name_;
};

inline const optional<std::size_t>& async_writer_memento::get_chunk_size() const
{
    return chunk_size_;
}

inline const optional<bool>& async_writer_memento::get_flush_on_destruct() const
{
    return flush_on_destruct_;
}

inline const optional<std::size_t>& async_writer_memento::get_max_chunks() const
{
    return max_chunks_;
}

inline const std::string& async_writer_memento::get_name() const
{
    return name_;
}

inline std::unique_ptr<writer>& async_writer_memento::get_writer()
{
    return writer_;
}

}

#endif
