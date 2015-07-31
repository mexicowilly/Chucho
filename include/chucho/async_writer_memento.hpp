/*
 * Copyright 2013-2015 Will Mason
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
#include <chucho/level.hpp>

namespace chucho
{

class async_writer_memento : public memento
{
public:
    async_writer_memento(configurator& cfg);

    std::shared_ptr<level> get_discard_threshold() const;
    const optional<bool>& get_flush_on_destruct() const;
    const optional<std::size_t>& get_queue_capacity() const;
    std::shared_ptr<writer> get_writer() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<level> discard_threshold_;
    optional<std::size_t> queue_capacity_;
    std::shared_ptr<writer> writer_;
    optional<bool> flush_on_destruct_;
};

inline std::shared_ptr<level> async_writer_memento::get_discard_threshold() const
{
    return discard_threshold_;
}

inline const optional<bool>& async_writer_memento::get_flush_on_destruct() const
{
    return flush_on_destruct_;
}

inline const optional<std::size_t>& async_writer_memento::get_queue_capacity() const
{
    return queue_capacity_;
}

inline std::shared_ptr<writer> async_writer_memento::get_writer() const
{
    return writer_;
}

}

#endif
