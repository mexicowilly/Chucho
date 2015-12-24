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

#include <chucho/pipe_writer.hpp>
#include <chucho/exception.hpp>
#include "error_util.hpp"

namespace chucho
{

pipe_writer::pipe_writer(std::shared_ptr<formatter> fmt,
                         bool flsh)
    : file_descriptor_writer(fmt, flsh),
      input_(INVALID_HANDLE_VALUE),
      output_(INVALID_HANDLE_VALUE)
{
    if (CreatePipe(&input_,
                   &output_,
                   NULL,
                   0))
    {
        set_file_handle(output_);
    }
    else
    {
        throw exception("Could not create pipes: " + error_util::message(GetLastError()));
    }
}

pipe_writer::~pipe_writer()
{
    CloseHandle(input_);
}

}
