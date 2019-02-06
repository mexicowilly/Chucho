/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/interval_file_roll_trigger.hpp>
#include <chucho/calendar.hpp>

namespace chucho
{

interval_file_roll_trigger::interval_file_roll_trigger(period prd, std::size_t count)
    : period_(prd),
      count_(count)
{
    compute_next_roll();
}

void interval_file_roll_trigger::compute_next_roll()
{
    calendar::pieces pcs = calendar::get_utc(clock_type::to_time_t(clock_type::now()));
    if (period_ == period::MINUTE)
    {
        pcs.tm_sec = 0;
        pcs.tm_min += static_cast<int>(count_);
    }
    else if (period_ == period::HOUR)
    {
        pcs.tm_sec = 0;
        pcs.tm_min = 0;
        pcs.tm_hour += static_cast<int>(count_);
    }
    else if (period_ == period::DAY)
    {
        pcs.tm_sec = 0;
        pcs.tm_min = 0;
        pcs.tm_hour = 0;
        pcs.tm_mday += static_cast<int>(count_);
    }
    else if (period_ == period::WEEK)
    {
        pcs.tm_sec = 0;
        pcs.tm_min = 0;
        pcs.tm_hour = 0;
        pcs.tm_mday += static_cast<int>(count_ * 7);
    }
    else if (period_ == period::MONTH)
    {
        pcs.tm_sec = 0;
        pcs.tm_min = 0;
        pcs.tm_hour = 0;
        pcs.tm_mday = 1;
        pcs.tm_mon += static_cast<int>(count_);
    }
    next_roll_ = clock_type::from_time_t(calendar::to_time_t(pcs));
}

bool interval_file_roll_trigger::is_triggered(const std::string& active_file, const event& e)
{
    if (clock_type::now() >= next_roll_)
    {
        compute_next_roll();
        return true;
    }
    return false;
}

}
