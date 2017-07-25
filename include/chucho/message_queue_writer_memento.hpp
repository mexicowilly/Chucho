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

#if !defined(CHUCHO_MESSAGE_QUEUE_WRITER_MEMENTO_HPP__)
#define CHUCHO_MESSAGE_QUEUE_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/serializer.hpp>
#include <chucho/compressor.hpp>

namespace chucho
{

class message_queue_writer_memento : public writer_memento
{
public:
    message_queue_writer_memento(configurator& cfg);

    std::shared_ptr<compressor> get_compressor() const;
    std::shared_ptr<serializer> get_serializer() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<serializer> serializer_;
    std::shared_ptr<compressor> compressor_;
};

inline std::shared_ptr<compressor> message_queue_writer_memento::get_compressor() const
{
    return compressor_;
}

inline std::shared_ptr<serializer> message_queue_writer_memento::get_serializer() const
{
    return serializer_;
}

}

#endif
