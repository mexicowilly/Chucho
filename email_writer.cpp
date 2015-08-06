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
#include <chucho/exception.hpp>
#include <chucho/calendar.hpp>
#include <chucho/pattern_formatter.hpp>
#include <sstream>
#include <iomanip>

namespace
{

std::once_flag once;

class curl_exception : public chucho::exception
{
public:
    curl_exception(CURLcode err, const std::string& msg);
};

struct read_data
{
    std::string message;
    std::string::size_type pos;
};

std::size_t curl_read_cb(char* buf, std::size_t sz, std::size_t nitems, void* data)
{
    read_data* rd = static_cast<read_data*>(data);
    if (rd->pos == rd->message.length())
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

curl_exception::curl_exception(CURLcode err, const std::string& msg)
    : chucho::exception(msg + ": " + curl_easy_strerror(err))
{
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
      subject_(subject)
{
    std::call_once(once, curl_global_init, CURL_GLOBAL_ALL);
    set_status_origin("email_writer");
    curl_ = curl_easy_init();
    if (curl_ == nullptr)
        throw exception("Could not initialize the CURL library");
    try
    {
        std::ostringstream stream;
        stream << "smtp://" << host_ << ':' << port_;
        CURLcode rc = curl_easy_setopt(curl_, CURLOPT_URL, stream.str().c_str());
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL URL");
        rc = curl_easy_setopt(curl_, CURLOPT_MAIL_FROM, from_.c_str());
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL mail from field");
        struct curl_slist* rcpts = nullptr;
        for (auto one : to_)
            rcpts = curl_slist_append(rcpts, one.c_str());
        rc = curl_easy_setopt(curl_, CURLOPT_MAIL_RCPT, rcpts);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL mail to field");
        rc = curl_easy_setopt(curl_, CURLOPT_UPLOAD, 1);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL upload option");
        rc = curl_easy_setopt(curl_, CURLOPT_READFUNCTION, curl_read_cb);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL read callback");

        #if !defined(NDEBUG)

        rc = curl_easy_setopt(curl_, CURLOPT_DEBUGFUNCTION, curl_debug_callback);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL debug callback");
        rc = curl_easy_setopt(curl_, CURLOPT_DEBUGDATA, this);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL debug user data pointer");
        rc = curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL verbose output");

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
    stream << std::setfill('0') << ENG_DAY[p.tm_wday] << ", " << p.tm_mday <<
        ENG_MON[p.tm_mon] << ' ' << (p.tm_year + 1900) << std::setw(2) << p.tm_hour <<
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
        stream << to_[i];
        if (i != to_.size() - 1)
            stream << ',';
    }
    stream << smtpl;
    stream << "From: " << from_ << smtpl;
    stream << "Message-ID: 1" << smtpl;
    pattern_formatter sub_fmt(subject_);
    stream << "Subject: " << sub_fmt.format(evt) << smtpl;
    stream << smtpl; // Empty line required to separate header from body
    while (!evts_.empty())
    {
        stream << formatter_->format(evts_.front()) << smtpl;
        evts_.pop();
    }
    return stream.str();
}

void email_writer::write_impl(const event& evt)
{
    evts_.push(evt);
    if (trigger_->is_triggered(evt))
    {
        read_data rd;
        rd.message = format_message(evt);
        rd.pos = 0;
        CURLcode rc = curl_easy_setopt(curl_, CURLOPT_READDATA, &rd);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set CURL read data");
        rc = curl_easy_perform(curl_);
        if (rc != CURLE_OK)
            throw curl_exception(rc, "Could not set send email");
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

}