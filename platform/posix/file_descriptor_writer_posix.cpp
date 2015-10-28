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
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <errno.h>

namespace chucho
{

file_descriptor_writer::file_descriptor_writer(std::shared_ptr<formatter> fmt,
                                               bool flsh)
    : writer(fmt),
      num_(0),
      fd_(-1),
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
      fd_(fd),
      flush_(flsh),
      allow_close_(true)
{
    set_status_origin("file_descriptor_writer");
}

void file_descriptor_writer::close()
{
    if (fd_ != -1)
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
            ::close(fd_);
        fd_ = -1;
    }
}

void file_descriptor_writer::flush()
{
    if (fd_ == -1)
    {
        report_warning("The file descriptor is not set, so this writer is discarding its data");
        num_ = 0;
    }
    else
    {
        auto rc = ::write(fd_, buf_.data(), num_);
        if (rc == -1)
        {
            int err = errno;
            throw exception("An error occurred writing to file descriptor " + std::to_string(fd_) + ": " + std::strerror(err));
        }
        if (num_ > rc)
            std::memmove(buf_.data(), buf_.data() + rc, num_ - rc);
        num_ -= rc;
    }
}

void file_descriptor_writer::set_file_descriptor(int fd)
{
    if (fd < 1)
        throw std::invalid_argument("The file descriptor must be greater than zero");
    fd_ = fd;
}

}