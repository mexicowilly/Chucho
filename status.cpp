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

#include <chucho/status.hpp>
#include <chucho/calendar.hpp>
#include <chucho/exception.hpp>
#include <chucho/clock_util.hpp>
#include <sstream>
#include <iomanip>

namespace chucho
{

status::status(level lvl,
               const std::string& message,
               const std::string& origin,
               std::exception_ptr ex)
    : time_(clock_type::now()),
      level_(lvl),
      message_(message),
      exception_(ex),
      origin_(origin)
{
}

std::ostream& operator<< (std::ostream& stream, const status& st)
{
    if (clock_util::system_clock_supports_milliseconds)
    {
        auto since = st.time_.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since);
        std::ostringstream fmt_stream;
        fmt_stream << "%H.%M.%S." << (millis.count() % 1000);
        struct std::tm cal = calendar::get_utc(millis.count() / 1000);
        stream << calendar::format(cal, fmt_stream.str());
    }
    else
    {
        struct std::tm cal = calendar::get_utc(status::clock_type::to_time_t(st.time_));
        stream << calendar::format(cal, "%H:%M:%S");

    }
    stream << ' ';
    if (st.level_ == status::level::INFO)
        stream << "INFO";
    else if (st.level_ == status::level::WARNING)
        stream << "WARN";
    else if (st.level_ == status::level::ERROR)
        stream << "ERROR";
    if (!st.origin_.empty())
        stream << " [" << st.origin_ << ']';
    stream << ' ' << st.message_;
    if (st.exception_ != std::exception_ptr())
    {
        stream << std::endl << "  exception: ";
        try
        {
            std::rethrow_exception(st.exception_);
        }
        catch (exception& e)
        {
            stream << exception::nested_whats(e);
        }
        catch (std::exception& se)
        {
            stream << se.what();
        }
    }
    return stream;
}

}
