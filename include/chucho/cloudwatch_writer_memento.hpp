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

#if !defined(CHUCHO_CLOUDWATCH_WRITER_MEMENTO_HPP__)
#define CHUCHO_CLOUDWATCH_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class cloudwatch_writer_memento : public writer_memento
{
public:
    cloudwatch_writer_memento(configurator& cfg);

    const optional<std::size_t>& get_batch_size() const;
    const std::string& get_log_group() const;
    const std::string& get_log_stream() const;
    const std::string& get_region() const;

private:
    optional<std::size_t> batch_size_;
    std::string log_group_;
    std::string log_stream_;
    std::string region_;
};

inline const optional<std::size_t>& cloudwatch_writer_memento::get_batch_size() const
{
    return batch_size_;
}

inline const std::string& cloudwatch_writer_memento::get_log_group() const
{
    return log_group_;
}

inline const std::string& cloudwatch_writer_memento::get_log_stream() const
{
    return log_stream_;
}

inline const std::string& cloudwatch_writer_memento::get_region() const
{
    return region_;
}

}

#endif
