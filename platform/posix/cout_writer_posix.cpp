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

#include <chucho/cout_writer.hpp>
#include <unistd.h>

namespace chucho
{

cout_writer::cout_writer(const std::string& name, std::unique_ptr<formatter>&& fmt)
    : file_descriptor_writer(name, std::move(fmt), STDOUT_FILENO, true)
{
    set_allow_close(false);
}

}
