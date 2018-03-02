/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/curl.hpp>
#include <chucho/garbage_cleaner.hpp>

namespace
{

std::once_flag global_once;

void global_setup()
{
    curl_global_init(CURL_GLOBAL_ALL);
    chucho::garbage_cleaner::get().add([] () { curl_global_cleanup(); });
}

}

namespace chucho
{

int curl_debug_callback(CURL* crl,
                        curl_infotype info_type,
                        char* text,
                        std::size_t num,
                        void* user_data)
{
    static_cast<curl*>(user_data)->report_info(std::string(text, num));
    return 0;
}

curl::curl()

    : curl_(nullptr),
      verbose_(false)
{
    std::call_once(global_once, global_setup);
    curl_ = curl_easy_init();
    if (curl_ == nullptr)
        throw std::runtime_error("Could nott initialize libcurl");
}

curl::~curl()
{
    if (curl_ != nullptr)
        curl_easy_cleanup(curl_);
}

bool curl::get_ssl_supported()
{
    std::call_once(global_once, global_setup);
    curl_version_info_data *ver = curl_version_info(CURLVERSION_NOW);
    return (ver->features & CURL_VERSION_SSL) != 0;
}

bool curl::get_verbose() const
{
    std::lock_guard<std::mutex> lock(guard_);
    return verbose_;
}

void curl::set_verbose(bool state)
{
    std::lock_guard<std::mutex> lock(guard_);
    if (state && !verbose_)
    {
        set_option(CURLOPT_DEBUGFUNCTION, curl_debug_callback, "debug callback");
        set_option(CURLOPT_DEBUGDATA, this, "debug user data pointer");
        set_option(CURLOPT_VERBOSE, 1, "verbose output");
    }
    else if (!state && verbose_)
    {
        set_option(CURLOPT_DEBUGFUNCTION, nullptr, "debug callback");
        set_option(CURLOPT_DEBUGDATA, nullptr, "debug user data pointer");
        set_option(CURLOPT_VERBOSE, 0, "verbose output");
    }
    verbose_ = state;
}

curl::curl_exception::curl_exception(CURLcode err, const std::string& msg)
    : chucho::exception(msg + ": " + curl_easy_strerror(err))
{
}

}
