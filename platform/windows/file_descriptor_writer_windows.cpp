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

#include <chucho/file_descriptor_writer.hpp>
#include <chucho/exception.hpp>
#include "error_util.hpp"
#include <stdexcept>
#include <io.h>

namespace chucho
{

file_descriptor_writer::file_descriptor_writer(std::shared_ptr<formatter> fmt,
                                               bool flsh)
    : writer(fmt),
      num_(0),
      handle_(INVALID_HANDLE_VALUE),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

file_descriptor_writer::file_descriptor_writer(std::shared_ptr<formatter> fmt,
                                               int fd,
                                               bool flsh)
    : writer(fmt),
      num_(0),
      handle_(reinterpret_cast<HANDLE>(_get_osfhandle(fd))),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

file_descriptor_writer::file_descriptor_writer(std::shared_ptr<formatter> fmt,
                                               HANDLE hnd,
                                               bool flsh)
    : writer(fmt),
      num_(0),
      handle_(hnd),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

void file_descriptor_writer::close()
{
    if (handle_ != INVALID_HANDLE_VALUE)
    {
        try
        {
            if (num_ > 0)
                flush();
        }
        catch (std::exception& e)
        {
            report_error(std::string("An error occurred while flushing on close: ") + e.what());
        }
        if (allow_close_)
            CloseHandle(handle_);
        handle_ = INVALID_HANDLE_VALUE;
    }
}

void file_descriptor_writer::flush()
{
    if (handle_ == INVALID_HANDLE_VALUE)
    {
        report_warning("The handle is not set, so this writer is discarding its data");
        num_ = 0;
    }
    else
    {
        DWORD written;
        if (!WriteFile(handle_,
                       buf_.data(),
                       num_,
                       &written,
                       NULL))
        {
            throw exception("An error occurred writing to file: " + error_util::message(GetLastError()));
        }
        if (num_ > written)
            std::memmove(buf_.data(), buf_.data() + written, num_ - written);
        num_ -= written;
    }
}

void file_descriptor_writer::set_file_descriptor(int fd)
{
    set_file_handle(reinterpret_cast<HANDLE>(_get_osfhandle(fd)));
}

void file_descriptor_writer::set_file_handle(HANDLE hnd)
{
    if (hnd == INVALID_HANDLE_VALUE)
        throw std::invalid_argument("The handle must be valid");
    handle_ = hnd;
}

}
