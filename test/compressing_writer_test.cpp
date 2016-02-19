/*
 * Copyright 2013-2016 Will Mason
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
#include <chucho/compressing_writer.hpp>
#include <chucho/noop_compressor.hpp>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <sstream>
#include <fstream>
#if defined(CHUCHO_POSIX)
#include <arpa/inet.h>
#endif

namespace
{

class compressing_writer_test : public ::testing::Test
{
public:
    std::shared_ptr<chucho::compressing_writer> get_writer(const chucho::optional<std::size_t>& max_kb = chucho::compressing_writer::DEFAULT_CACHE_SIZE_IN_KB,
                                                           const chucho::optional<std::size_t>& max_evts = chucho::optional<std::size_t>())
    {
        auto fmt = std::make_shared<chucho::pattern_formatter>("%m");
        auto ser = std::make_shared<chucho::formatted_message_serializer>();
        auto cmp = std::make_shared<chucho::noop_compressor>();
        auto str = std::make_shared<std::ostringstream>();
        auto wrt = std::make_shared<chucho::compressing_writer>(fmt, str, cmp, ser, max_kb, max_evts);
        return wrt;
    }

    void write_event(std::shared_ptr<chucho::writer> wrt, const char* const msg)
    {
        chucho::event evt(chucho::logger::get("compressing_writer_test"),
                          chucho::level::INFO_(),
                          msg,
                          __FILE__,
                          __LINE__,
                          __FUNCTION__);
        wrt->write(evt);
    }
};

}

TEST_F(compressing_writer_test, accessors)
{
    auto wrt = get_writer(700, 701);
    auto cmp_cast = std::dynamic_pointer_cast<chucho::noop_compressor>(wrt->get_compressor());
    EXPECT_NE(nullptr, cmp_cast.get());
    EXPECT_EQ(0, wrt->get_events_in_cache());
    ASSERT_TRUE(wrt->get_max_cached_bytes());
    EXPECT_EQ(700 * 1024, *wrt->get_max_cached_bytes());
    ASSERT_TRUE(wrt->get_max_cached_events());
    EXPECT_EQ(701, *wrt->get_max_cached_events());
    auto ser_cast = std::dynamic_pointer_cast<chucho::formatted_message_serializer>(wrt->get_serializer());
    EXPECT_NE(nullptr, ser_cast.get());
    auto str_cast = std::dynamic_pointer_cast<std::ostringstream>(wrt->get_stream());
    EXPECT_NE(nullptr, str_cast.get());
    auto fstr = std::make_shared<std::ofstream>();
    wrt->set_stream(fstr);
    auto fstr_cast = std::dynamic_pointer_cast<std::ofstream>(wrt->get_stream());
    EXPECT_NE(nullptr, fstr_cast.get());
}

TEST_F(compressing_writer_test, basic)
{
    auto wrt = get_writer();
    write_event(wrt, "My dog");
    write_event(wrt, "Has fleas");
    EXPECT_EQ(2, wrt->get_events_in_cache());
    auto stream = std::dynamic_pointer_cast<std::ostringstream>(wrt->get_stream());
    EXPECT_TRUE(stream->str().empty());
    wrt.reset();
    EXPECT_FALSE(stream->str().empty());
    auto out = stream->str();
    EXPECT_EQ(23, out.length());
    std::uint32_t len;
    out.copy(reinterpret_cast<char*>(&len), 4);
    len = ntohl(len);
    EXPECT_EQ(6, len);
    auto sub = out.substr(4, 6);
    EXPECT_STREQ("My dog", sub.c_str());
    out.erase(0, 10);
    out.copy(reinterpret_cast<char*>(&len), 4);
    len = ntohl(len);
    EXPECT_EQ(9, len);
    sub = out.substr(4, 9);
    EXPECT_STREQ("Has fleas", sub.c_str());
}

TEST_F(compressing_writer_test, byte_size)
{
    auto wrt = get_writer(1);
    ASSERT_TRUE(wrt->get_max_cached_bytes());
    EXPECT_EQ(1024, *wrt->get_max_cached_bytes());
    EXPECT_FALSE(wrt->get_max_cached_events());
    auto stream = std::dynamic_pointer_cast<std::ostringstream>(wrt->get_stream());
    std::string big( 512, 'W');
    write_event(wrt, big.c_str());
    EXPECT_TRUE(stream->str().empty());
    EXPECT_EQ(1, wrt->get_events_in_cache());
    big.append(200, 'M');
    write_event(wrt, big.c_str());
    EXPECT_FALSE(stream->str().empty());
    EXPECT_EQ(0, wrt->get_events_in_cache());
}

TEST_F(compressing_writer_test, event_count)
{
    auto wrt = get_writer(chucho::optional<std::size_t>(), 2);
    ASSERT_TRUE(wrt->get_max_cached_events());
    EXPECT_EQ(2, *wrt->get_max_cached_events());
    EXPECT_FALSE(wrt->get_max_cached_bytes());
    auto stream = std::dynamic_pointer_cast<std::ostringstream>(wrt->get_stream());
    write_event(wrt, "one");
    EXPECT_TRUE(stream->str().empty());
    EXPECT_EQ(1, wrt->get_events_in_cache());
    write_event(wrt, "two");
    EXPECT_FALSE(stream->str().empty());
    EXPECT_EQ(0, wrt->get_events_in_cache());
}