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

#include <chucho/clock_util.hpp>
#include <chrono>

namespace chucho
{

namespace clock_util
{

const bool system_clock_supports_milliseconds =
    (static_cast<double>(std::chrono::system_clock::period::num) / static_cast<double>(std::chrono::system_clock::period::den))
    <=
    (1.0 / 1000.0);

}

}
