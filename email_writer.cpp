/*
 * Copyright 2013-2021 Will Mason
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
#include <chucho/curl.hpp>
#include "fnv.h"
#include <sstream>
#include <iomanip>
#include <atomic>
#include <algorithm>

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

const std::uint16_t email_writer::DEFAULT_PORT(25);
const std::size_t email_writer::DEFAULT_BUFFER_CAPACITY(256);

email_writer::email_writer(const std::string& name,
                           std::unique_ptr<formatter>&& fmt,
                           const std::string& host,
                           connection_type connect,
                           const std::vector<std::string>& to,
                           const std::string& from,
                           const std::string& subject,
                           std::unique_ptr<email_trigger>&& trigger,
                           std::uint16_t port,
                           std::size_t buffer_size)
    : writer(name, std::move(fmt)),
      evts_(buffer_size),
      curl_(std::make_unique<curl>()),
      trigger_(std::move(trigger)),
      from_(from),
      to_(to),
      host_(host),
      port_(port),
      subject_(subject),
      connection_type_(connect)
{
    init();
}

email_writer::email_writer(const std::string& name,
                           std::unique_ptr<formatter>&& fmt,
                           const std::string& host,
                           connection_type connect,
                           const std::vector<std::string>& to,
                           const std::string& from,
                           const std::string& subject,
                           std::unique_ptr<email_trigger>&& trigger,
                           const std::string& user,
                           const std::string& password,
                           std::uint16_t port,
                           std::size_t buffer_size)
    : writer(name, std::move(fmt)),
      evts_(buffer_size),
      curl_(std::make_unique<curl>()),
      trigger_(std::move(trigger)),
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
    // Don't delete this destructor.
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

bool email_writer::get_ssl_supported()
{
    return curl::get_ssl_supported();
}

bool email_writer::get_verbose() const
{
    return curl_->get_verbose();
}

void email_writer::init()
{
    if (!trigger_)
        throw std::invalid_argument("The trigger cannot be uninitialized");
    set_status_origin("email_writer");
    if (!curl::get_ssl_supported() && connection_type_ != connection_type::CLEAR)
    {
        std::string type_text = (connection_type_ == connection_type::SSL) ?
            "SSL" : "STARTTLS";
        report_error("The connection type " + type_text + " is not supported because the CURL library on the system does not support SSL");
        throw exception("[email_writer] Unsupported connection type: " + type_text);
    }
    try
    {
        std::ostringstream stream;
        if (connection_type_ == connection_type::SSL)
            stream << "smtps";
        else
            stream << "smtp";
        stream << "://" << host_ << ':' << port_;
        curl_->set_option(CURLOPT_URL, stream.str().c_str(), "URL");
        stream.str("");
        stream << '<' << from_ << '>';
        curl_->set_option(CURLOPT_MAIL_FROM, stream.str().c_str(), "mail from field");
        std::vector<std::string> tos;
        std::transform(to_.begin(), to_.end(), std::back_inserter(tos), [] (const std::string& s) { return '<' + s + '>'; });
        curl_->set_option(CURLOPT_MAIL_RCPT, curl_->create_slist(std::move(tos)), "mail to");
        curl_->set_option(CURLOPT_UPLOAD, 1, "upload");
        curl_->set_option(CURLOPT_READFUNCTION, curl_read_cb, "read callback");
        if (user_)
            curl_->set_option(CURLOPT_USERNAME, user_->c_str(), "user name");
        if (password_)
            curl_->set_option(CURLOPT_PASSWORD, password_->c_str(), "password");
        if (connection_type_ == connection_type::STARTTLS)
            curl_->set_option(CURLOPT_USE_SSL, CURLUSESSL_ALL, "use SSL (for STARTTLS connection type");
        if (connection_type_ == connection_type::STARTTLS || connection_type_ == connection_type::SSL)
        {
            curl_->set_option(CURLOPT_SSL_VERIFYPEER, 0, "not to verify the peer certificate");
            curl_->set_option(CURLOPT_SSL_VERIFYHOST, 0, "not to verify the host");
        }

        #if !defined(NDEBUG)
        set_verbose(true);
        #endif
    }
    catch (exception&)
    {
        curl_.reset();
        throw;
    }
}

void email_writer::set_verbose(bool state)
{
    curl_->set_verbose(state);
}

void email_writer::write_impl(const event& evt)
{
    evts_.push(evt);
    if (trigger_->is_triggered(evt))
    {
        read_data rd;
        rd.message = format_message(evt);
        rd.pos = 0;
        curl_->set_option(CURLOPT_READDATA, &rd, "read user data");
        CURLcode rc = curl_easy_perform(curl_->get());
        if (rc != CURLE_OK)
            throw exception(std::string("Could not send email: ") + curl_easy_strerror(rc));
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
