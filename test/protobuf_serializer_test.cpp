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

#include <gtest/gtest.h>
#include <chucho/protobuf_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
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
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m");
    chucho::protobuf_serializer ser;
    auto res = ser.serialize(evt, fmt);
    chucho::proto::event pevt;
    pevt.ParseFromArray(&res[0], res.size());
    EXPECT_EQ(evt.get_message(), pevt.formatted_message());
    EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), pevt.seconds_since_epoch());
    EXPECT_STREQ(evt.get_file_name(), pevt.file_name().c_str());
    EXPECT_EQ(evt.get_line_number(), pevt.line_number());
    EXPECT_STREQ(evt.get_function_name(), pevt.function_name().c_str());
    EXPECT_EQ(evt.get_logger()->get_name(), pevt.logger());
    EXPECT_STREQ(evt.get_level()->get_name(), pevt.level_name().c_str());
    EXPECT_FALSE(pevt.has_marker());
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    EXPECT_EQ(stream.str(), pevt.thread());
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
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m");
    chucho::protobuf_serializer ser;
    auto res = ser.serialize(evt, fmt);
    chucho::proto::event pevt;
    pevt.ParseFromArray(&res[0], res.size());
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