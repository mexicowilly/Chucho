/*
 * Copyright 2013-2018 Will Mason
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

namespace
{

void noop_invalid_parameter(const wchar_t*,
                            const wchar_t*,
                            const wchar_t*,
                            unsigned int,
                            uintptr_t)
{
}

HANDLE get_handle_from_fd(int fd)
{
    _invalid_parameter_handler hnd = _set_invalid_parameter_handler(noop_invalid_parameter);
    HANDLE result = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
    _set_invalid_parameter_handler(hnd);
    return result;
}

}

namespace chucho
{

file_descriptor_writer::file_descriptor_writer(const std::string& name,
                                               std::unique_ptr<formatter>&& fmt,
                                               bool flsh)
    : writer(name, std::move(fmt)),
      num_(0),
      handle_(INVALID_HANDLE_VALUE),
      fd_(-1),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

file_descriptor_writer::file_descriptor_writer(const std::string& name,
                                               std::unique_ptr<formatter>&& fmt,
                                               int fd,
                                               bool flsh)
    : writer(name, std::move(fmt)),
      num_(0),
      handle_(get_handle_from_fd(fd)),
      fd_(fd),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

file_descriptor_writer::file_descriptor_writer(const std::string& name,
                                               std::unique_ptr<formatter>&& fmt,
                                               HANDLE hnd,
                                               bool flsh)
    : writer(name, std::move(fmt)),
      num_(0),
      handle_(hnd),
      fd_(-1),
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
        {
            if (fd_ == -1)
                CloseHandle(handle_);
            else
                _close(fd_);
        }
        handle_ = INVALID_HANDLE_VALUE;
        fd_ = -1;
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
                       static_cast<DWORD>(num_),
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
    fd_ = fd;
    set_file_handle(get_handle_from_fd(fd));
}

void file_descriptor_writer::set_file_handle(HANDLE hnd)
{
    if (hnd == INVALID_HANDLE_VALUE)
        throw std::invalid_argument("The handle must be valid");
    handle_ = hnd;
}

}
