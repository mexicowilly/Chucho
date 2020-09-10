/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/log_streambuf.hpp>
#include <chucho/event.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

log_streambuf::log_streambuf(std::shared_ptr<logger> lgr)
    : logger_(lgr),
      file_name_(""),
      line_number_(0),
      function_name_("")
{
    if (!lgr)
        throw exception("log_streambuf: The logger is empty");
    set_status_origin("log_streambuf"); 
}
    
void log_streambuf::flush_message()
{
    if (!level_)
    {
        report_warning("Cannot write the message because the level has not been set");
    }
    else if (logger_->permits(level_))
    {
        event evt(logger_,
                  level_,
                  message_,
                  file_name_,
                  line_number_,
                  function_name_,
                  marker_);
        logger_->write(evt);
    }
    message_.clear();
    file_name_ = "";
    line_number_ = 0;
    function_name_ = "";
    marker_ = optional<marker>();
}
    

log_streambuf::int_type log_streambuf::overflow(int_type ch)
{
    if (ch != traits_type::eof())
        message_ += traits_type::to_char_type(ch);
    return ch;
}

void log_streambuf::set_location(const char *const file_name,
                                 unsigned int line_number,
                                 const char *const function_name)
{
    file_name_ = file_name;
    line_number_ = line_number;
    function_name_ = function_name;
}

}
