/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/file_roller.hpp>
#include <chucho/file_writer.hpp>

namespace chucho
{

file_roller::file_roller(std::unique_ptr<file_compressor>&& cmp)
    : file_writer_(nullptr),
      compressor_(std::move(cmp))
{
}

file_roller::~file_roller()
{
}

void file_roller::set_file_writer(file_writer& file_writer)
{
    if (file_writer_ != nullptr)
        report_warning("The file_roller already has a file_writer, which is being replaced");
    file_writer_ = &file_writer;
}

}
