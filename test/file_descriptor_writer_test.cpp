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

#include <gtest/gtest.h>
#include <chucho/file_descriptor_writer.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <fstream>
#if defined(CHUCHO_POSIX)
#include <fcntl.h>
#endif

namespace
{

class file_descriptor_writer_test : public ::testing::Test
{
public:
    file_descriptor_writer_test()
        : file_name_("file_descriptor_writer")
    {
#if defined(CHUCHO_POSIX)
        int flag = O_WRONLY | O_TRUNC | O_CREAT;
        fd_ = open(file_name_.c_str(), flag, 0644);
        if (fd_ == -1)
            throw chucho::exception("Could not open " + file_name_ + " for writing");
#elif defined(CHUCHO_WINDOWS)
        fd_ = CreateFileA(file_name_.c_str(),
                          GENERIC_WRITE,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          NULL,
                          CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL);
        if (fd_ == INVALID_HANDLE_VALUE)
            throw chucho::exception("Could not open " + file_name_ + " for writing");
#endif
    }

    ~file_descriptor_writer_test()
    {
        chucho::file::remove(file_name_);
    }

    std::unique_ptr<chucho::file_descriptor_writer> get_writer(bool flsh)
    {
        auto f = std::make_unique<chucho::pattern_formatter>("%m");
        auto w = std::make_unique<chucho::file_descriptor_writer>("fd", std::move(f), fd_, flsh);
        return std::move(w);
    }

    void verify(std::size_t count)
    {
        ASSERT_EQ(count, chucho::file::size(file_name_));
        std::ifstream stream(file_name_);
        std::string cnt;
        stream >> cnt;
        EXPECT_EQ(count, cnt.length());
        char ch = '!';
        for (auto i : cnt)
        {
            ASSERT_EQ(ch++, i);
            if (ch > '~')
                ch = '!';
        }
    }

    void write(std::unique_ptr<chucho::file_descriptor_writer>& w, std::size_t count)
    {
        std::shared_ptr<chucho::logger> log = chucho::logger::get("file_descriptor_writer_test");
        std::string s;
        char ch = '!';
        for (std::size_t i = 0; i < count; i++)
        {
            s += ch++;
            if (ch > '~')
                ch = '!';
        }
        chucho::event evt(log, chucho::level::INFO_(), s, __FILE__, __LINE__, __FUNCTION__);
        w->write(evt);
    }

protected:
    std::string file_name_;

private:
#if defined(CHUCHO_POSIX)
    int fd_;
#elif defined(CHUCHO_WINDOWS)
    HANDLE fd_;
#endif
};

}

TEST_F(file_descriptor_writer_test, big_flush)
{
    auto w = get_writer(true);
    write(w, 10 * 1024);
    verify(10 * 1024);
}

TEST_F(file_descriptor_writer_test, big_no_flush)
{
    auto w = get_writer(false);
    write(w, 10 * 1024);
    EXPECT_EQ(8 * 1024, chucho::file::size(file_name_));
    w.reset();
    verify(10 * 1024);
}

TEST_F(file_descriptor_writer_test, small_flush)
{
    auto w = get_writer(true);
    write(w, 4 * 1024);
    verify(4 * 1024);
}

TEST_F(file_descriptor_writer_test, small_no_flush)
{
    auto w = get_writer(false);
    write(w, 4 * 1024);
    EXPECT_EQ(0, chucho::file::size(file_name_));
    w.reset();
    verify(4 * 1024);
}
