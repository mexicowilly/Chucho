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

#include <gtest/gtest.h>
#include <chucho/flatbuffers_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include "chucho_generated.h"
#include <thread>
#include <sstream>

TEST(flatbuffers_serializer, no_marker)
{
chucho::event evt(chucho::logger::get("capn_proto_serializer_test"),
                  chucho::level::TRACE_(),
                  "Hi",
                  __FILE__,
                  __LINE__,
                  __FUNCTION__);
    auto fmt = chucho::pattern_formatter("%m");
    chucho::flatbuffers_serializer ser;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 100; j++)
            ser.serialize(evt, fmt);
        auto res = ser.finish_blob();
        auto log_events = chucho::flatb::Getlog_events(res.data());
        flatbuffers::Verifier ver(res.data(), res.size());
        ASSERT_EQ(100, log_events->events()->size());
        ASSERT_FALSE(log_events->host() == nullptr);
        EXPECT_STREQ(chucho::host::get_full_name().c_str(), log_events->host()->c_str());
        ASSERT_TRUE(log_events->Verify(ver));
        for (int j = 0; j < 100; j++)
        {
            auto fevt = log_events->events()->Get(j);
            fevt->Verify(ver);
            ASSERT_FALSE(fevt->formatted_message() == nullptr);
            EXPECT_STREQ(evt.get_message().c_str(), fevt->formatted_message()->c_str());
            EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), fevt->seconds_since_epoch());
            ASSERT_FALSE(fevt->file_name() == nullptr);
            EXPECT_STREQ(evt.get_file_name(), fevt->file_name()->c_str());
            EXPECT_EQ(evt.get_line_number(), fevt->line_number());
            ASSERT_FALSE(fevt->function_name() == nullptr);
            EXPECT_STREQ(evt.get_function_name(), fevt->function_name()->c_str());
            ASSERT_FALSE(fevt->logger() == nullptr);
            EXPECT_STREQ(evt.get_logger()->get_name().c_str(), fevt->logger()->c_str());
            ASSERT_FALSE(fevt->level_name() == nullptr);
            EXPECT_STREQ(evt.get_level()->get_name(), fevt->level_name()->c_str());
            EXPECT_TRUE(fevt->marker() == nullptr);
            ASSERT_FALSE(fevt->thread() == nullptr);
            std::ostringstream tstream;
            tstream << std::this_thread::get_id();
            EXPECT_STREQ(tstream.str().c_str(), fevt->thread()->c_str());
        }
    }
}

TEST(flatbuffers_serializer, with_marker)
{
    chucho::event evt(chucho::logger::get("protobuf_serializer_test"),
                      chucho::level::TRACE_(),
                      "Hi",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__,
                      "marky");
    auto fmt = chucho::pattern_formatter("%m");
    chucho::flatbuffers_serializer ser;
    ser.serialize(evt, fmt);
    auto res = ser.finish_blob();
    auto log_events = chucho::flatb::Getlog_events(res.data());
    flatbuffers::Verifier ver(res.data(), res.size());
    ASSERT_EQ(1, log_events->events()->size());
    ASSERT_TRUE(log_events->Verify(ver));
    auto fevt = log_events->events()->Get(0);
    fevt->Verify(ver);
    ASSERT_FALSE(fevt->formatted_message() == nullptr);
    EXPECT_STREQ(evt.get_message().c_str(), fevt->formatted_message()->c_str());
    EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), fevt->seconds_since_epoch());
    ASSERT_FALSE(fevt->file_name() == nullptr);
    EXPECT_STREQ(evt.get_file_name(), fevt->file_name()->c_str());
    EXPECT_EQ(evt.get_line_number(), fevt->line_number());
    ASSERT_FALSE(fevt->function_name() == nullptr);
    EXPECT_STREQ(evt.get_function_name(), fevt->function_name()->c_str());
    ASSERT_FALSE(fevt->logger() == nullptr);
    EXPECT_STREQ(evt.get_logger()->get_name().c_str(), fevt->logger()->c_str());
    ASSERT_FALSE(fevt->level_name() == nullptr);
    EXPECT_STREQ(evt.get_level()->get_name(), fevt->level_name()->c_str());
    ASSERT_FALSE(fevt->marker() == nullptr);
    EXPECT_STREQ(evt.get_marker()->get_name().c_str(), fevt->marker()->c_str());
    ASSERT_FALSE(fevt->thread() == nullptr);
    std::ostringstream tstream;
    tstream << std::this_thread::get_id();
    EXPECT_STREQ(tstream.str().c_str(), fevt->thread()->c_str());
}
