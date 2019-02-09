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

#include <chucho/pipe_writer.hpp>
#include <chucho/exception.hpp>
#include <cstring>
#include <errno.h>
#include <unistd.h>

namespace chucho
{

pipe_writer::pipe_writer(const std::string& name,
                         std::unique_ptr<formatter>&& fmt,
                         bool flsh)
    : file_descriptor_writer(name, std::move(fmt), flsh),
      input_(-1),
      output_(-1)
{
    int fds[2];
    if (pipe(fds) == -1)
    {
        int err = errno;
        throw exception(std::string("Could not create pipes: ") + std::strerror(err));
    }
    input_ = fds[0];
    output_ = fds[1];
    set_file_descriptor(output_);
}

pipe_writer::~pipe_writer()
{
    ::close(input_);
}

}
