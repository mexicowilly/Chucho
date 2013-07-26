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

#include <chucho/file_writer.hpp>
#include <chucho/file_exception.hpp>
#include <chucho/file.hpp>

namespace chucho
{

file_writer::file_writer(std::shared_ptr<formatter> fmt,
                         on_start start,
                         bool flush)
    : writer(fmt),
      start_(start),
      flush_(flush)
{
    set_status_origin("file_writer");
}

file_writer::file_writer(std::shared_ptr<formatter> fmt,
                         const std::string& file_name,
                         on_start start,
                         bool flush)
    : writer(fmt),
      initial_file_name_(file_name),
      start_(start),
      flush_(flush)
{
    set_status_origin("file_writer");
    open(file_name);
}

void file_writer::open(const std::string& file_name)
{
    file_name_ = file_name;
    try
    {
        file::create_directories(file::directory_name(file_name));
        file_.open(file_name, std::ios::out | ((start_ == on_start::APPEND) ? std::ios::app : std::ios::trunc));
        if (file_.is_open())
            file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
        else
            report_error("Could not open " + file_name + " for writing");
    }
    catch (std::exception& e)
    {
        report_error("Error creating parent directories of " + file_name + ": " + e.what());
    }
}

void file_writer::write_impl(const event& evt)
{
    try
    {
        if (file_.is_open())
        {
            file_ << formatter_->format(evt);
            if (flush_)
                file_.flush();
        }
        else
        {
            report_error("Cannot write to " + file_name_ + " because it is not open");
        }
    }
    catch (std::ios::failure&)
    {
#if defined(CHUCHO_HAVE_NESTED_EXCEPTIONS)
        std::throw_with_nested(file_exception("Could not write to " + file_name_));
#else
        throw file_exception("Could not write to " + file_name_);
#endif
    }
}

}
