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

#include <chucho/interval_file_roll_trigger_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/text_util.hpp>
#include <sstream>
#include <cctype>

namespace chucho
{

interval_file_roll_trigger_memento::interval_file_roll_trigger_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("interval_file_roll_trigger_memento");
    cfg.get_security_policy().set_integer("interval_file_roll_trigger::count", 1U, 5000U);
    cfg.get_security_policy().set_text("interval_file_roll_trigger::period(text)", 12);
    set_handler("every", std::bind(&interval_file_roll_trigger_memento::parse_period, this, std::placeholders::_1));
}

void interval_file_roll_trigger_memento::parse_period(const std::string& spec)
{
    validate("interval_file_roll_trigger::period(text)", spec);
    if (spec.empty() || !std::isdigit(spec[0]))
        throw exception(get_status_origin() + ": The period specification must start with a digit");
    std::istringstream in(spec);
    std::size_t count;
    in >> count;
    validate("interval_file_roll_trigger::count", count);
    std::string raw;
    in >> raw;
    std::string period = text_util::to_lower(raw);
    if (period.length() < 3)
        throw exception(get_status_origin() + ": The period must be minutes, hours, days, weeks or months. This one is \"" + raw + "\"");
    if (period.back() == 's')
        period.pop_back();
    if (period == "minute")
        period_ = interval_file_roll_trigger::period::MINUTE;
    else if (period == "hour")
        period_ = interval_file_roll_trigger::period::HOUR;
    else if (period == "day")
        period_ = interval_file_roll_trigger::period::DAY;
    else if (period == "week")
        period_ = interval_file_roll_trigger::period::WEEK;
    else if (period == "month")
        period_ = interval_file_roll_trigger::period::MONTH;
    else
        throw exception(get_status_origin() + ": The period must be minutes, hours, days, weeks or months. This one is \"" + raw + "\"");
}

}
