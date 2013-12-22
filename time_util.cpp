/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/time_util.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <thread>

namespace
{

struct static_data
{
    static_data();

    std::chrono::steady_clock::time_point start_;
};

static_data::static_data()
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
    // This will be cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

namespace chucho
{

std::chrono::milliseconds::rep time_util::milliseconds_since_start()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
       std::chrono::steady_clock::now() - data().start_).count();
}

void time_util::start_now()
{
    data().start_ = std::chrono::steady_clock::now();
}

}
