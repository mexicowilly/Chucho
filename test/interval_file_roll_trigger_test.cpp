/*
 * Copyright 2013-2014 Will Mason
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

#include <gtest/gtest.h>
#include <chucho/interval_file_roll_trigger.hpp>
#include <chucho/calendar.hpp>

typedef chucho::interval_file_roll_trigger::clock_type clock_type;

TEST(interval_file_roll_trigger, day)
{
    chucho::interval_file_roll_trigger trg(chucho::interval_file_roll_trigger::period::DAY, 100);
    auto cal = chucho::calendar::get_local(clock_type::to_time_t(clock_type::now()));
    cal.tm_sec = 0;
    cal.tm_min = 0;
    cal.tm_hour = 0;
    cal.tm_mday += 100;
    EXPECT_EQ(clock_type::from_time_t(std::mktime(&cal)), trg.get_next_roll());
}

TEST(interval_file_roll_trigger, hour)
{
    chucho::interval_file_roll_trigger trg(chucho::interval_file_roll_trigger::period::HOUR, 7);
    auto cal = chucho::calendar::get_local(clock_type::to_time_t(clock_type::now()));
    cal.tm_sec = 0;
    cal.tm_min = 0;
    cal.tm_hour += 7;
    EXPECT_EQ(clock_type::from_time_t(std::mktime(&cal)), trg.get_next_roll());
}

TEST(interval_file_roll_trigger, minute)
{
    chucho::interval_file_roll_trigger trg(chucho::interval_file_roll_trigger::period::MINUTE, 5);
    auto cal = chucho::calendar::get_local(clock_type::to_time_t(clock_type::now()));
    cal.tm_sec = 0;
    cal.tm_min += 5;
    EXPECT_EQ(clock_type::from_time_t(std::mktime(&cal)), trg.get_next_roll());
}

TEST(interval_file_roll_trigger, month)
{
    chucho::interval_file_roll_trigger trg(chucho::interval_file_roll_trigger::period::MONTH, 10);
    auto cal = chucho::calendar::get_local(clock_type::to_time_t(clock_type::now()));
    cal.tm_sec = 0;
    cal.tm_min = 0;
    cal.tm_hour = 0;
    cal.tm_mday = 1;
    cal.tm_mon += 10;
    EXPECT_EQ(clock_type::from_time_t(std::mktime(&cal)), trg.get_next_roll());
}

TEST(interval_file_roll_trigger, week)
{
    chucho::interval_file_roll_trigger trg(chucho::interval_file_roll_trigger::period::WEEK, 2);
    auto cal = chucho::calendar::get_local(clock_type::to_time_t(clock_type::now()));
    cal.tm_sec = 0;
    cal.tm_min = 0;
    cal.tm_hour = 0;
    cal.tm_mday += 2 * 7;
    EXPECT_EQ(clock_type::from_time_t(std::mktime(&cal)), trg.get_next_roll());
}
