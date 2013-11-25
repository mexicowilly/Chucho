/*
 * Copyright 2013 Will Mason
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

#include <chucho/pattern_formatter.hpp>
#include <time.h>
#include <unistd.h>

namespace chucho
{

std::string pattern_formatter::milliseconds_since_start_piece::get_text_impl(const event& evt) const
{
    clock_t clocks = clock();
    std::string result;
    if (clocks != -1)
        result = std::to_string(clocks / (CLOCKS_PER_SEC / 1000));
    return result;
}

std::string pattern_formatter::pid_piece::get_text_impl(const event& evt) const
{
    return std::to_string(getpid());
}

}
