/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_PIPE_WRITER_MEMENTO_HPP__)
#define CHUCHO_PIPE_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class pipe_writer_memento : public writer_memento
{
public:
    pipe_writer_memento(configurator& cfg);

    const optional<bool>& get_flush() const;

private:
    optional<bool> flush_;
};

inline const optional<bool>& pipe_writer_memento::get_flush() const
{
    return flush_;
}

}

#endif
