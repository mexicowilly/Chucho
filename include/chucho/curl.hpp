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
#ifndef CHUCHO_CURL_HPP__
#define CHUCHO_CURL_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/export.h>
#include <chucho/status_reporter.hpp>
#include <chucho/exception.hpp>
#include <curl/curl.h>
#include <cstdint>
#include <string>
#include <mutex>

namespace chucho
{

class CHUCHO_PRIV_EXPORT curl : public status_reporter
{
public:
    static bool get_ssl_supported();

    curl();
    virtual ~curl();

    CURL* get() const;
    std::mutex& get_guard();
    bool get_verbose() const;
    template<typename arg_type>
    void set_option(CURLoption opt, arg_type arg, const char* const err_msg);
    void set_verbose(bool state);

private:
    class CHUCHO_NO_EXPORT curl_exception : public chucho::exception
    {
    public:
        curl_exception(CURLcode err, const std::string& msg);
    };

    friend CHUCHO_NO_EXPORT int curl_debug_callback(CURL* crl,
                                                    curl_infotype info_type,
                                                    char* text,
                                                    std::size_t num,
                                                    void* user_data);

    CURL* curl_;
    bool verbose_;
    mutable std::mutex guard_;
};

inline CURL* curl::get() const
{
    return curl_;
}

inline std::mutex& curl::get_guard()
{
    return guard_;
}

template<typename arg_type>
void curl::set_option(CURLoption opt, arg_type arg, const char* const err_msg)
{
    CURLcode rc = curl_easy_setopt(curl_, opt, arg);
    if (rc != CURLE_OK)
        throw curl_exception(rc, std::string("Could not set CURL option ") + err_msg);
}

}

#endif
