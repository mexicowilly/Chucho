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

#include <chucho/file_writer.hpp>
#include <errno.h>
#include <cstring>
#include <fcntl.h>

namespace chucho
{

void file_writer::open_impl(const std::string& file_name)
{
    int flag = O_WRONLY;
    #if defined(CHUCHO_HAVE_O_LARGEFILE)
    flag |= O_LARGEFILE;
    #endif
    if (allow_creation_)
        flag |= O_CREAT;
    if (has_been_opened_ || start_ == on_start::APPEND)
        flag |= O_APPEND;
    else
        flag |= O_TRUNC;
    int fd = ::open(file_name.c_str(), flag, 0644);
    if (fd == -1)
    {
        int err = errno;
        report_error("Unable to open " + file_name + " for writing: " + std::strerror(err));
        is_open_ = false;
    }
    else
    {
        set_file_descriptor(fd);
        is_open_ = true;
    }
}

}
