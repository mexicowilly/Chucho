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

namespace chucho
{

file_descriptor_writer::~file_descriptor_writer()
{
    close();
}

void file_descriptor_writer::write_impl(const event& evt)
{
    std::string msg = formatter_->format(evt);
    std::size_t left = msg.length();
    while (left > 0)
    {
        std::size_t to_copy = std::min(left, buf_.size() - num_);
        msg.copy(buf_.data() + num_, to_copy, msg.length() - left);
        left -= to_copy;
        num_ += to_copy;
        if (num_ == buf_.size())
            flush();
    }
    if (flush_ && num_ > 0)
        flush();
}

}