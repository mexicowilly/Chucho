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

#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/file.hpp>

namespace chucho
{

size_file_roll_trigger::size_file_roll_trigger(std::uintmax_t max_size)
    : max_size_(max_size)
{
    set_status_origin("size_file_roll_trigger");
}

bool size_file_roll_trigger::is_triggered(const std::string& active_file, const event& e)
{
    try
    {
        return file::size(active_file) >= max_size_;
    }
    catch (...)
    {
    }
    return false;
}

}
