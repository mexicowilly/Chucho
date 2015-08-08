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

#include <chucho/email_writer.hpp>
#include <chucho/calendar.hpp>
#include <chucho/pattern_formatter.hpp>
#include "fnv.h"
#include <sstream>
#include <iomanip>

namespace
{

struct read_data
{
    std::string message;
    std::string::size_type pos;
};

std::size_t curl_read_cb(char* buf, std::size_t sz, std::size_t nitems, void* data)
{
    read_data* rd = static_cast<read_data*>(data);
    if (rd->pos >= rd->message.length())
        return 0;
    std::size_t to_copy = std::min(rd->message.length() - rd->pos, sz * nitems);
    rd->message.copy(buf, to_copy, rd->pos);
    rd->pos += to_copy;
    return to_copy;
}

inline std::ostream& smtpl(std::ostream& stream)
{
    stream << "\r\n";
    return stream;
}

}

namespace chucho
{

int curl_debug_callback(CURL* curl,
                        curl_infotype info_type,
                        char* text,
                        std::size_t num,
                        void* user_data)
{
    static_cast<email_writer*>(user_data)->report_info(std::string(text, num));
    return 0;
}

email_writer::email_writer(std::shared_ptr<formatter> fmt,
                           const std::string& host,
                           connection_type connect,
                           const std::vector<std::string>& to,
                           const std::string& from,
                           const std::string& subject,
                           std::shared_ptr<email_trigger> trigger,
                           std::uint16_t port,
                           std::size_t buffer_size)
    : writer(fmt),
      evts_(buffer_size),
      curl_(nullptr),
      trigger_(trigger),
      from_(from),
      to_(to),
      host_(host),
      port_(port),
      subject_(subject),
      connection_type_(connect)
{
    init();
}

email_writer::email_writer(std::shared_ptr<formatter> fmt,
                           const std::string& host,
                           connection_type connect,
                           const std::vector<std::string>& to,
                           const std::string& from,
                           const std::string& subject,
                           std::shared_ptr<email_trigger> trigger,
                           const std::string& user,
                           const std::string& password,
                           std::uint16_t port,
                           std::size_t buffer_size)
    : writer(fmt),
      evts_(buffer_size),
      curl_(nullptr),
      trigger_(trigger),
      from_(from),
      to_(to),
      host_(host),
      port_(port),
      subject_(subject),
      user_(user),
      password_(password),
      connection_type_(connect)
{
    init();
}

email_writer::~email_writer()
{
    if (curl_ != nullptr)
        curl_easy_cleanup(curl_);
}

std::string email_writer::format_date() const
{
    static const char* ENG_DAY[] =
    {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static const char* ENG_MON[] =
    {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    calendar::pieces p = calendar::get_local(std::time(nullptr));
    std::ostringstream stream;
    stream << std::setfill('0') << ENG_DAY[p.tm_wday] << ", " << p.tm_mday << ' ' <<
        ENG_MON[p.tm_mon] << ' ' << (p.tm_year + 1900) << ' ' << std::setw(2) << p.tm_hour <<
        ':' << std::setw(2) << p.tm_min << ':' << std::setw(2) << p.tm_sec << ' ';
    long tz = calendar::get_time_zone_offset_in_minutes();
    if (tz < 0)
    {
        stream << '-';
        tz = -tz;
    }
    else
    {
        stream << '+';
    }
    stream << std::setw(2) << (tz / 60) << std::setw(2) << (tz % 60);
    return stream.str();
}

std::string email_writer::format_message(const event& evt)
{
    std::ostringstream stream;
    stream << "Date: " << format_date() << smtpl;
    stream << "To: ";
    for (unsigned i = 0; i < to_.size(); i++)
    {
        stream << '<' << to_[i] << '>';
        if (i != to_.size() - 1)
            stream << ',';
    }
    stream << smtpl;
    stream << "From: <" << from_ << '>' << smtpl;
    pattern_formatter sub_fmt(subject_);
    stream << "Subject: " << sub_fmt.format(evt) << smtpl;
    stream << smtpl; // Empty line required to separate header from body
    while (!evts_.empty())
    {
        stream << formatter_->format(evts_.front()) << smtpl;
        evts_.pop();
    }
    std::string msg = stream.str();
    Fnv64_t fnv = fnv_64a_str(const_cast<char*>(msg.c_str()), FNV1A_64_INIT);
    stream.str("");
    stream << "Message-ID: " << std::setfill('0') << std::hex << std::setw(16) <<
        fnv << '-' << std::setw(16) << std::time(nullptr) << smtpl;
    msg.insert(0, stream.str());
    return msg;
}

void email_writer::init()
{
    static std::once_flag once;

    std::call_once(once, curl_global_init, CURL_GLOBAL_ALL);
    set_status_origin("email_writer");
    curl_ = curl_easy_init();
    if (curl_ == nullptr)
        throw exception("Could not initialize the CURL library");
    try
    {
        std::ostringstream stream;
        if (connection_type_ == connection_type::SSL)
            stream << "smtps";
        else
            stream << "smtp";
        stream << "://" << host_ << ':' << port_;
        set_curl_option(CURLOPT_URL, stream.str().c_str(), "URL");
        stream.str("");
        stream << '<' << from_ << '>';
        set_curl_option(CURLOPT_MAIL_FROM, stream.str().c_str(), "mail from field");
        struct curl_slist* rcpts = nullptr;
        for (auto one : to_)
        {
            stream.str("");
            stream << '<' << one << '>';
            rcpts = curl_slist_append(rcpts, stream.str().c_str());
        }
        set_curl_option(CURLOPT_MAIL_RCPT, rcpts, "mail to");
        set_curl_option(CURLOPT_UPLOAD, 1, "upload");
        set_curl_option(CURLOPT_READFUNCTION, curl_read_cb, "read callback");
        if (user_)
            set_curl_option(CURLOPT_USERNAME, user_->c_str(), "user name");
        if (password_)
            set_curl_option(CURLOPT_PASSWORD, password_->c_str(), "password");
        if (connection_type_ == connection_type::STARTTLS)
            set_curl_option(CURLOPT_USE_SSL, CURLUSESSL_ALL, "use SSL (for STARTTLS connection type");
        if (connection_type_ == connection_type::STARTTLS || connection_type_ == connection_type::SSL)
        {
            set_curl_option(CURLOPT_SSL_VERIFYPEER, 0, "not to verify the peer certificate");
            set_curl_option(CURLOPT_SSL_VERIFYHOST, 0, "not to verify the host");
        }

        #if !defined(NDEBUG)

        set_curl_option(CURLOPT_DEBUGFUNCTION, curl_debug_callback, "debug callback");
        set_curl_option(CURLOPT_DEBUGDATA, this, "debug user data pointer");
        set_curl_option(CURLOPT_VERBOSE, 1, "verbose output");

        #endif
    }
    catch (exception&)
    {
        if (curl_ != nullptr)
        {
            curl_easy_cleanup(curl_);
            curl_ = nullptr;
        }
        throw;
    }
}

void email_writer::write_impl(const event& evt)
{
    evts_.push(evt);
    if (trigger_->is_triggered(evt))
    {
        read_data rd;
        rd.message = format_message(evt);
        rd.pos = 0;
        set_curl_option(CURLOPT_READDATA, &rd, "read user data");
        CURLcode rc = curl_easy_perform(curl_);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not send email");
    }
}

email_writer::fixed_size_queue::fixed_size_queue(std::size_t max)
    : max_(max)
{
}

void email_writer::fixed_size_queue::push(const event& evt)
{
    while (q_.size() >= max_)
        q_.pop();
    q_.push(evt);
}

email_writer::curl_exception::curl_exception(CURLcode err, const std::string& msg)
    : chucho::exception(msg + ": " + curl_easy_strerror(err))
{
}

}