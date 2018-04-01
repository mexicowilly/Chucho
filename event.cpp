/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/event.hpp>
#include <chucho/marker.hpp>

namespace chucho
{

event::event(std::shared_ptr<logger> lgr,
             std::shared_ptr<level> lvl,
             const std::string& msg,
             const char* const file_name,
             unsigned line_number,
             const char* const function_name,
             const optional<marker>& mark)
    : logger_(lgr),
      level_(lvl),
      message_(msg),
      time_(clock_type::now()),
      file_name_(file_name),
      line_number_(line_number),
      function_name_(function_name),
      marker_(mark)
{
}

event::event(std::shared_ptr<logger> lgr,
             std::shared_ptr<level> lvl,
             const std::string& msg,
             const char* const file_name,
             unsigned line_number,
             const char* const function_name,
             const std::string& mark)
    : logger_(lgr),
      level_(lvl),
      message_(msg),
      time_(clock_type::now()),
      file_name_(file_name),
      line_number_(line_number),
      function_name_(function_name),
      marker_(mark)
{
}

event::event(const event& evt)
    : logger_(evt.logger_),
      level_(evt.level_),
      message_(evt.message_),
      time_(evt.time_),
      file_name_(evt.file_name_),
      line_number_(evt.line_number_),
      function_name_(evt.function_name_),
      marker_(evt.marker_),
      thread_id_(evt.thread_id_)
{
    if (evt.file_name_store_)
    {
        file_name_store_ = evt.file_name_store_;
        file_name_ = file_name_store_->c_str();
    }
    if (evt.function_name_store_)
    {
        function_name_store_ = evt.function_name_store_;
        function_name_ = function_name_store_->c_str();
    }
}

event& event::operator= (const event& evt)
{
    logger_ = evt.logger_;
    level_ = evt.level_;
    message_ = evt.message_;
    time_ = evt.time_;
    if (evt.file_name_store_)
    {
        file_name_store_ = evt.file_name_store_;
        file_name_ = file_name_store_->c_str();
    }
    else
    {
        file_name_ = evt.file_name_;
    }
    line_number_ = evt.line_number_;
    if (evt.function_name_store_)
    {
        function_name_store_ = evt.function_name_store_;
        function_name_ = function_name_store_->c_str();
    }
    else
    {
        function_name_ = evt.function_name_;
    }
    marker_ = evt.marker_;
    thread_id_ = evt.thread_id_;
    return *this;
}

}
