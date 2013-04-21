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
#include <chrono>
#include <thread>

namespace
{

const std::chrono::minutes CACHE_DURATION(5);

std::chrono::steady_clock::time_point expiration(std::chrono::steady_clock::now());
std::mutex guard;

}

namespace chucho
{

std::string host::base_;
std::string host::full_;

const std::string& host::get_base_name()
{
    std::lock_guard<std::mutex> lg(guard);
    if (std::chrono::steady_clock::now() >= expiration)
    {
        get_base_impl();
        expiration += CACHE_DURATION;
    }
    return base_;
}

const std::string& host::get_full_name()
{
    std::lock_guard<std::mutex> lg(guard);
    if (std::chrono::steady_clock::now() >= expiration)
    {
        get_full_impl();
        expiration += CACHE_DURATION;
    }
    return full_;
}

}
