/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_EMAIL_WRITER_HPP__)
#define CHUCHO_EMAIL_WRITER_HPP__

#include <chucho/writer.hpp>
#include <chucho/email_trigger.hpp>
#include <chucho/optional.hpp>
#include <chucho/exception.hpp>
#include <queue>
#include <curl/curl.h>

namespace chucho
{

class CHUCHO_EXPORT email_writer : public writer
{
public:
    enum class connection_type
    {
        CLEAR,
        STARTTLS,
        SSL
    };

    email_writer(std::shared_ptr<formatter> fmt,
                 const std::string& host,
                 connection_type connect,
                 const std::vector<std::string>& to,
                 const std::string& from,
                 const std::string& subject,
                 std::shared_ptr<email_trigger> trigger,
                 std::uint16_t port = 25,
                 std::size_t buffer_capacity = 256);
    email_writer(std::shared_ptr<formatter> fmt,
                 const std::string& host,
                 connection_type connect,
                 const std::vector<std::string>& to,
                 const std::string& from,
                 const std::string& subject,
                 std::shared_ptr<email_trigger> trigger,
                 const std::string& user,
                 const std::string& password,
                 std::uint16_t port = 25,
                 std::size_t buffer_capacity = 256);
    virtual ~email_writer();

    std::size_t get_buffer_capacity() const;
    std::size_t get_buffer_size() const;
    const std::string& get_from() const;
    const std::string& get_host() const;
    const optional<std::string>& get_password() const;
    std::uint16_t get_port() const;
    const std::string& get_subject() const;
    const std::vector<std::string>& get_to() const;
    std::shared_ptr<email_trigger> get_trigger() const;
    const optional<std::string>& get_user() const;

protected:
    friend CHUCHO_NO_EXPORT int curl_debug_callback(CURL* curl,
                                                    curl_infotype info_type,
                                                    char* text,
                                                    std::size_t num,
                                                    void* user_data);

    virtual void write_impl(const event& evt) override;

private:
    class CHUCHO_NO_EXPORT fixed_size_queue
    {
    public:
        fixed_size_queue(std::size_t max);

        std::size_t capacity() const;
        bool empty() const;
        const event& front() const;
        void pop();
        void push(const event& evt);
        std::size_t size() const;

    private:
        std::queue<event> q_;
        std::size_t max_;
    };

    CHUCHO_NO_EXPORT class curl_exception : public chucho::exception
    {
    public:
        curl_exception(CURLcode err, const std::string& msg);
    };

    CHUCHO_NO_EXPORT std::string format_date() const;
    CHUCHO_NO_EXPORT std::string format_message(const event& evt);
    CHUCHO_NO_EXPORT void init(connection_type connect);
    template<typename arg_type>
    CHUCHO_NO_EXPORT void set_curl_option(CURLoption opt, arg_type arg, const char* const err_msg);

    fixed_size_queue evts_;
    CURL* curl_;
    std::shared_ptr<email_trigger> trigger_;
    std::string from_;
    std::vector<std::string> to_;
    std::string host_;
    std::uint16_t port_;
    std::string subject_;
    optional<std::string> user_;
    optional<std::string> password_;
};

inline std::size_t email_writer::get_buffer_capacity() const
{
    return evts_.capacity();
}

inline std::size_t email_writer::get_buffer_size() const
{
    return evts_.size();
}

inline const std::string& email_writer::get_from() const
{
    return from_;
}

inline const std::string& email_writer::get_host() const
{
    return host_;
}

inline const optional<std::string>& email_writer::get_password() const
{
    return password_;
}

inline std::uint16_t email_writer::get_port() const
{
    return port_;
}

inline const std::vector<std::string>& email_writer::get_to() const
{
    return to_;
}

inline const std::string& email_writer::get_subject() const
{
    return subject_;
}

inline std::shared_ptr<email_trigger> email_writer::get_trigger() const
{
    return trigger_;
}

inline const optional<std::string>& email_writer::get_user() const
{
    return user_;
}

template<typename arg_type>
void email_writer::set_curl_option(CURLoption opt, arg_type arg, const char* const err_msg)
{
    CURLcode rc = curl_easy_setopt(curl_, opt, arg);
    if (rc != CURLE_OK)
        throw curl_exception(rc, std::string("Could not set CURL option ") + err_msg);
}

inline std::size_t email_writer::fixed_size_queue::capacity() const
{
    return max_;
}

inline bool email_writer::fixed_size_queue::empty() const
{
    return q_.empty();
}

inline const event& email_writer::fixed_size_queue::front() const
{
    return q_.front();
}

inline void email_writer::fixed_size_queue::pop()
{
    q_.pop();
}

inline std::size_t email_writer::fixed_size_queue::size() const
{
    return q_.size();
}

}

#endif