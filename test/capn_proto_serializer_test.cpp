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
#include <chucho/capn_proto_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include "chucho.capnp.h"
#include <capnp/serialize.h>
#include <thread>
#include <sstream>

TEST(capn_proto_serializer, no_marker)
{
    chucho::event evt(chucho::logger::get("capn_proto_serializer_test"),
                      chucho::level::TRACE_(),
                      "Hi",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    auto fmt = chucho::pattern_formatter("%m");
    chucho::capn_proto_serializer ser;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 100; j++)
            ser.serialize(evt, fmt);
        auto res = ser.finish_blob();
        kj::ArrayPtr<capnp::word> ptr(reinterpret_cast<capnp::word *>(&res[0]), res.size() / sizeof(capnp::word));
        capnp::FlatArrayMessageReader reader(ptr);
        chucho::capnp::Events::Reader cevts = reader.getRoot<chucho::capnp::Events>();
        ASSERT_EQ(100, cevts.getEvents().size());
        ASSERT_TRUE(cevts.hasHostName());
        EXPECT_STREQ(chucho::host::get_full_name().c_str(), cevts.getHostName().cStr());
        for (int j = 0; j < 100; j++)
        {
            auto cevt = cevts.getEvents()[j];
            EXPECT_TRUE(cevt.hasFormattedMessage());
            EXPECT_STREQ(evt.get_message().c_str(), cevt.getFormattedMessage().cStr());
            EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), cevt.getSecondsSinceEpoch());
            EXPECT_TRUE(cevt.hasFileName());
            EXPECT_STREQ(evt.get_file_name(), cevt.getFileName().cStr());
            EXPECT_EQ(evt.get_line_number(), cevt.getLineNumber());
            EXPECT_TRUE(cevt.hasFunctionName());
            EXPECT_STREQ(evt.get_function_name(), cevt.getFunctionName().cStr());
            EXPECT_TRUE(cevt.hasLogger());
            EXPECT_STREQ(evt.get_logger()->get_name().c_str(), cevt.getLogger().cStr());
            EXPECT_TRUE(cevt.hasLevelName());
            EXPECT_STREQ(evt.get_level()->get_name(), cevt.getLevelName().cStr());
            EXPECT_FALSE(cevt.hasMarker());
            EXPECT_TRUE(cevt.hasThread());
            std::ostringstream tstream;
            tstream << std::this_thread::get_id();
            EXPECT_STREQ(tstream.str().c_str(), cevt.getThread().cStr());
        }
    }
}

TEST(capn_proto_serializer, with_marker)
{
    chucho::event evt(chucho::logger::get("protobuf_serializer_test"),
                      chucho::level::TRACE_(),
                      "Hi",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__,
                      "marky");
    auto fmt = chucho::pattern_formatter("%m");
    chucho::capn_proto_serializer ser;
    ser.serialize(evt, fmt);
    auto res = ser.finish_blob();
    kj::ArrayPtr<capnp::word> ptr(reinterpret_cast<capnp::word*>(&res[0]), res.size() / 8);
    capnp::FlatArrayMessageReader reader(ptr);
    chucho::capnp::Events::Reader cevts = reader.getRoot<chucho::capnp::Events>();
    ASSERT_EQ(1, cevts.getEvents().size());
    auto cevt = cevts.getEvents()[0];
    EXPECT_TRUE(cevt.hasFormattedMessage());
    EXPECT_STREQ(evt.get_message().c_str(), cevt.getFormattedMessage().cStr());
    EXPECT_EQ(chucho::event::clock_type::to_time_t(evt.get_time()), cevt.getSecondsSinceEpoch());
    EXPECT_TRUE(cevt.hasFileName());
    EXPECT_STREQ(evt.get_file_name(), cevt.getFileName().cStr());
    EXPECT_EQ(evt.get_line_number(), cevt.getLineNumber());
    EXPECT_TRUE(cevt.hasFunctionName());
    EXPECT_STREQ(evt.get_function_name(), cevt.getFunctionName().cStr());
    EXPECT_TRUE(cevt.hasLogger());
    EXPECT_STREQ(evt.get_logger()->get_name().c_str(), cevt.getLogger().cStr());
    EXPECT_TRUE(cevt.hasLevelName());
    EXPECT_STREQ(evt.get_level()->get_name(), cevt.getLevelName().cStr());
    EXPECT_TRUE(cevt.hasMarker());
    EXPECT_STREQ(evt.get_marker()->get_name().c_str(), cevt.getMarker().cStr());
    EXPECT_TRUE(cevt.hasThread());
    std::ostringstream tstream;
    tstream << std::this_thread::get_id();
    EXPECT_STREQ(tstream.str().c_str(), cevt.getThread().cStr());
}
