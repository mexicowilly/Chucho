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

#include <chucho/loggly_writer.hpp>
#include <chucho/curl.hpp>
#include <cJSON.h>
#include <cstring>

namespace
{

std::size_t written_cb(char* data, std::size_t sz, std::size_t num, void* user)
{
    auto str = reinterpret_cast<std::string*>(user);
    auto total = sz * num;
    str->append(data, total);
    return total;
}

}

namespace chucho
{

loggly_writer::loggly_writer(const std::string& name,
                             std::unique_ptr<formatter>&& fmt,
                             const std::string& token)
    : writer(name, std::move(fmt)),
      curl_(std::make_unique<curl>())
{
    auto url = "http://logs-01.loggly.com/inputs/" + token + "/tag/http";
    curl_->set_option(CURLOPT_URL, url.c_str(), "url");
    curl_->set_option(CURLOPT_HTTPHEADER, curl_->create_slist({"content-type:text/plain"}), "HTTP header");
    curl_->set_option(CURLOPT_WRITEFUNCTION, written_cb, "write funtion");
}

loggly_writer::~loggly_writer()
{
    // Don't delete this destructor
}

bool loggly_writer::get_verbose() const
{
    return curl_->get_verbose();
}

void loggly_writer::set_verbose(bool state)
{
    curl_->set_verbose(state);
}

void loggly_writer::write_impl(const event& evt)
{
    auto msg = formatter_->format(evt);
    curl_->set_option(CURLOPT_POSTFIELDS, msg.c_str(), "post fields");
    curl_->set_option(CURLOPT_POSTFIELDSIZE_LARGE, msg.length(), "post field size");
    std::string written;
    curl_->set_option(CURLOPT_WRITEDATA, &written, "write data");
    CURLcode rc = curl_easy_perform(curl_->get());
    if (rc != CURLE_OK)
        throw exception(std::string("Could not send event to Loggly: ") + curl_easy_strerror(rc));
    auto json = cJSON_Parse(written.c_str());
    if (json != nullptr)
    {
        try
        {
            auto resp = cJSON_GetObjectItemCaseSensitive(json, "response");
            if (resp == nullptr)
                throw exception("Unable to find response key in returned JSON");
            if (!cJSON_IsString(resp))
                throw exception("The JSON response is not a string");
            if (std::strcmp("ok", resp->valuestring) != 0)
                throw exception("Expected ok response, but got: " + std::string(resp->valuestring));
        } catch (...)
        {
            cJSON_Delete(json);
            throw;
        }
        cJSON_Delete(json);
    }
    else
    {
        throw exception("Unable to parse JSON from Loggly");
    }
}

}
