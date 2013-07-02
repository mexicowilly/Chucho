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

#include <chucho/host.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chrono>
#include <mutex>

namespace
{

struct static_data
{
    static_data();

    std::chrono::minutes cache_duration_;
    std::chrono::steady_clock::time_point expiration_;
    std::mutex guard_;
    std::string base_;
    std::string full_;
};

static_data::static_data()
    : cache_duration_(5),
      expiration_(std::chrono::steady_clock::now())
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
    // This gets cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

namespace chucho
{

const std::string& host::get_base_name()
{
    std::lock_guard<std::mutex> lg(data().guard_);
    static_data& sd(data());
    if (std::chrono::steady_clock::now() >= sd.expiration_)
    {
        get_base_impl(sd.base_);
        sd.expiration_ += sd.cache_duration_;
    }
    return sd.base_;
}

const std::string& host::get_full_name()
{
    std::lock_guard<std::mutex> lg(data().guard_);
    static_data& sd(data());
    if (std::chrono::steady_clock::now() >= sd.expiration_)
    {
        get_full_impl(sd.full_);
        sd.expiration_ += sd.cache_duration_;
    }
    return sd.full_;
}

}
