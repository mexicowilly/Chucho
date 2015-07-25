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

#if !defined(CHUCHO_FILE_WRITER_MEMENTO_HPP__)
#define CHUCHO_FILE_WRITER_MEMENTO_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/optional.hpp>
#include <chucho/memento_key_set.hpp>

namespace chucho
{

class file_writer_memento : public writer_memento
{
public:
    file_writer_memento(configurator& cfg, memento_key_set ks);

    const std::string& get_file_name() const;
    const optional<bool>& get_flush() const;
    const optional<file_writer::on_start>& get_on_start() const;

private:
    void set_on_start(const std::string& value);

    std::string file_name_;
    optional<file_writer::on_start> start_;
    optional<bool> flush_;
};

inline const std::string& file_writer_memento::get_file_name() const
{
    return file_name_;
}

inline const optional<bool>& file_writer_memento::get_flush() const
{
    return flush_;
}

inline const optional<file_writer::on_start>& file_writer_memento::get_on_start() const
{
    return start_;
}

}

#endif
