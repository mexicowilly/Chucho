/*
 * Copyright 2013 Will Mason
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

#include <chucho/numbered_file_roller.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <stdexcept>
#include <cstdio>
#include <sstream>

namespace chucho
{

// TODO: Forward this constructor when Microsoft decides to support that
numbered_file_roller::numbered_file_roller(int max_index)
    : min_index_(1),
      max_index_(max_index)
{
    if (max_index < 1)
        throw std::invalid_argument("numbered_file_roller: min_index must be less than or equal to max_index");
}

numbered_file_roller::numbered_file_roller(int min_index, int max_index)
    : min_index_(min_index),
      max_index_(max_index)
{
    if (min_index > max_index)
        throw std::invalid_argument("numbered_file_roller: min_index must be less than or equal to max_index");
    set_status_origin("numbered_file_roller");
}

std::string numbered_file_roller::get_active_file_name()
{
    return file_writer_->get_file_name();
}

std::string numbered_file_roller::get_name(int number) const
{
    std::ostringstream stream;
    stream << file_writer_->get_file_name() << '.' << number;
    return stream.str();
}

void numbered_file_roller::roll()
{
    try
    {
        file::remove(get_name(max_index_));
        for (int i = max_index_; i > min_index_; i--)
        {
            std::string name = get_name(i - 1);
            if (file::exists(name))
                std::rename(name.c_str(), get_name(i).c_str());
        }
        std::rename(file_writer_->get_file_name().c_str(),
                    get_name(min_index_).c_str());
    }
    catch (std::exception& e)
    {
#if defined(CHUCHO_HAVE_NESTED_EXCEPTIONS)
        std::throw_with_nested(exception("Could not roll the file " +
            file_writer_->get_file_name()));
#else
        throw exception(std::string(e.what()) +
            ": Could not roll the file " + file_writer_->get_file_name());
#endif
    }
}

}
