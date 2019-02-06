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

#if !defined(CHUCHO_ZEROMQ_WRITER_MEMENTO_HPP__)
#define CHUCHO_ZEROMQ_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/message_queue_writer_memento.hpp>

namespace chucho
{

class zeromq_writer_memento : public message_queue_writer_memento
{
public:
    zeromq_writer_memento(configurator& cfg);

    const std::string& get_endpoint() const;
    const std::vector<std::uint8_t>& get_prefix() const;

private:
    std::string endpoint_;
    std::vector<std::uint8_t> prefix_;
};

inline const std::string& zeromq_writer_memento::get_endpoint() const
{
    return endpoint_;
}

inline const std::vector<std::uint8_t>& zeromq_writer_memento::get_prefix() const
{
    return prefix_;
}

}

#endif
