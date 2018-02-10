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

}
