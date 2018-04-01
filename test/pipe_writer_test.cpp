/*
 * Copyright 2013-2018 Will Mason
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
#include <chucho/pipe_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <vector>
#if defined(CHUCHO_POSIX)
#include <unistd.h>
#endif

namespace
{

class pipe_writer_test : public ::testing::Test
{
public:
    pipe_writer_test()
      : writer_("pipe", std::move(std::make_unique<chucho::pattern_formatter>("%m")))
    {
    }

    std::string read(std::size_t num)
    {
        std::vector<char> buf(num);
        #if defined(CHUCHO_POSIX)
        auto actual = ::read(writer_.get_input(), &buf[0], num);
        #elif defined(CHUCHO_WINDOWS)
        DWORD actual;
        EXPECT_NE(0, ReadFile(writer_.get_input(), &buf[0], num, &actual, NULL));
        #endif
        EXPECT_EQ(num, actual);
        return std::string(&buf[0], num);
    }

    void write(const std::string& str)
    {
        std::shared_ptr<chucho::logger> log = chucho::logger::get("pipe_writer_test");
        chucho::event evt(log, chucho::level::INFO_(), str, __FILE__, __LINE__, __FUNCTION__);
        writer_.write(evt);
    }

private:
    chucho::pipe_writer writer_;
};

}

TEST_F(pipe_writer_test, simple)
{
    std::string s("hello");
    write(s);
    auto result = read(s.length());
    EXPECT_STREQ(s.c_str(), result.c_str());
}
