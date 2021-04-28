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

#include <gtest/gtest.h>
#include <chucho/protobuf_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include "chucho.pb.h"
#include <thread>
#include <sstream>

TEST(protobuf_serializer, no_marker)
{
    chucho::event evt(chucho::logger::get("protobuf_serializer_test"),
                      chucho::level::TRACE_(),
                      "Hi",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    auto fmt = chucho::pattern_formatter("%m");
    chucho::protobuf_serializer ser;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 100; j++)
            ser.serialize(evt, fmt);
        auto res = ser.finish_blob();
        chucho::proto::log_events pevts;
        pevts.ParseFromArray(&res[0], res.size());
        EXPECT_TRUE(pevts.has_host_name());
        EXPECT_EQ(chucho::host::get_full_name(), pevts.host_name());
        ASSERT_EQ(100, pevts.events_size());
        for (int j = 0; j < 100; j++)
        {
            auto pevt = pevts.events(0);
            EXPECT_TRUE(pevt.has_formatted_message());
            EXPECT_EQ(evt.get_message(), pevt.formatted_message());
            EXPECT_TRUE(pevt.has_seconds_since_epoch());
            EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), pevt.seconds_since_epoch());
            EXPECT_TRUE(pevt.has_file_name());
            EXPECT_STREQ(evt.get_file_name(), pevt.file_name().c_str());
            EXPECT_TRUE(pevt.has_line_number());
            EXPECT_EQ(evt.get_line_number(), pevt.line_number());
            EXPECT_TRUE(pevt.has_function_name());
            EXPECT_STREQ(evt.get_function_name(), pevt.function_name().c_str());
            EXPECT_TRUE(pevt.has_logger());
            EXPECT_EQ(evt.get_logger()->get_name(), pevt.logger());
            EXPECT_TRUE(pevt.has_level_name());
            EXPECT_STREQ(evt.get_level()->get_name(), pevt.level_name().c_str());
            EXPECT_FALSE(pevt.has_marker());
            EXPECT_TRUE(pevt.has_thread());
            std::ostringstream stream;
            stream << std::this_thread::get_id();
            EXPECT_EQ(stream.str(), pevt.thread());
        }
    }
}

TEST(protobuf_serializer, with_marker)
{
    chucho::event evt(chucho::logger::get("protobuf_serializer_test"),
                      chucho::level::TRACE_(),
                      "Hi",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__,
                      "marky");
    auto fmt = chucho::pattern_formatter("%m");
    chucho::protobuf_serializer ser;
    ser.serialize(evt, fmt);
    auto res = ser.finish_blob();
    chucho::proto::log_events pevts;
    pevts.ParseFromArray(&res[0], res.size());
    ASSERT_EQ(1, pevts.events_size());
    auto pevt = pevts.events(0);
    EXPECT_EQ(evt.get_message(), pevt.formatted_message());
    EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), pevt.seconds_since_epoch());
    EXPECT_STREQ(evt.get_file_name(), pevt.file_name().c_str());
    EXPECT_EQ(evt.get_line_number(), pevt.line_number());
    EXPECT_STREQ(evt.get_function_name(), pevt.function_name().c_str());
    EXPECT_EQ(evt.get_logger()->get_name(), pevt.logger());
    EXPECT_STREQ(evt.get_level()->get_name(), pevt.level_name().c_str());
    EXPECT_EQ(evt.get_marker()->get_name(), pevt.marker());
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_EQ(stream.str(), pevt.thread());
}
