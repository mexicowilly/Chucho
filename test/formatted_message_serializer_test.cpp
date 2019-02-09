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

#include <gtest/gtest.h>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>

TEST(formatted_message_serializer, simple)
{
    chucho::event evt(chucho::logger::get("protobuf_serializer_test"),
                      chucho::level::TRACE_(),
                      "Monkey Balls",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    chucho::pattern_formatter fmt("%m");
    chucho::formatted_message_serializer ser;
    ser.serialize(evt, fmt);
    auto res = ser.finish_blob();
    std::string seried(res.begin(), res.end());
    auto last = seried.find_last_not_of("\r\n") + 1;
    ASSERT_LT(last, seried.length());
    seried.erase(last);
    auto exp = fmt.format(evt);
    EXPECT_EQ(exp, seried);
}
