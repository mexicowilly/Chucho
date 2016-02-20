/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_COMPRESSING_WRITER_MEMENTO_HPP__)
#define CHUCHO_COMPRESSING_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/optional.hpp>
#include <chucho/compressor.hpp>
#include <chucho/serializer.hpp>

namespace chucho
{

class compressing_writer_memento : public writer_memento
{
public:
    compressing_writer_memento(configurator& cfg);

    std::shared_ptr<compressor> get_compressor() const;
    const optional<std::size_t>& get_max_cached_kb() const;
    const optional<std::size_t>& get_max_cached_events() const;
    std::shared_ptr<serializer> get_serializer() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<compressor> compressor_;
    optional<std::size_t> max_kb_;
    optional<std::size_t> max_events_;
    std::shared_ptr<serializer> serializer_;
};

inline std::shared_ptr<compressor> compressing_writer_memento::get_compressor() const
{
    return compressor_;
}

inline const optional<std::size_t>& compressing_writer_memento::get_max_cached_kb() const
{
    return max_kb_;
}

inline const optional<std::size_t>& compressing_writer_memento::get_max_cached_events() const
{
    return max_events_;
}

inline std::shared_ptr<serializer> compressing_writer_memento::get_serializer() const
{
    return serializer_;
}

}

#endif