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
#include <chucho/file_writer.hpp>
#include <chucho/file.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/status_manager.hpp>
#include <fstream>
#include <thread>
#if defined(CHUCHO_WINDOWS)
#include <windows.h>
#endif

namespace
{

class file_writer_test : public ::testing::Test
{
public:
    file_writer_test()
        : file_name_("file_writer_test")
    {
        chucho::file::remove(file_name_);
        chucho::status_manager::get().clear();
    }

    ~file_writer_test()
    {
        chucho::file::remove(file_name_);
    }

    std::unique_ptr<chucho::file_writer> get_writer(chucho::file_writer::on_start start = chucho::file_writer::on_start::APPEND)
    {
        auto f = std::make_unique<chucho::pattern_formatter>("%m%n");
        auto w = std::make_unique<chucho::file_writer>("fw", std::move(f), file_name_, start);
        return std::move(w);
    }

protected:
    std::string file_name_;
};

#if defined(CHUCHO_POSIX)

void make_unwriteable(const std::string& file_name)
{
    struct stat stat_buf;
    ASSERT_EQ(0, stat(file_name.c_str(), &stat_buf));
    ASSERT_EQ(0, chmod(file_name.c_str(), stat_buf.st_mode & ~(S_IWUSR | S_IWGRP | S_IWOTH)));
}

void make_writeable(const std::string& file_name)
{
    struct stat stat_buf;
    ASSERT_EQ(0, stat(file_name.c_str(), &stat_buf));
    ASSERT_EQ(0, chmod(file_name.c_str(), stat_buf.st_mode | (S_IWUSR | S_IWGRP | S_IWOTH)));
}

#elif defined(CHUCHO_WINDOWS)

void make_unwriteable(const std::string& file_name)
{
    ASSERT_EQ(TRUE,
        SetFileAttributesA(file_name.c_str(),
                           GetFileAttributesA(file_name.c_str()) | FILE_ATTRIBUTE_READONLY));
}

void make_writeable(const std::string& file_name)
{
    ASSERT_EQ(TRUE,
        SetFileAttributesA(file_name.c_str(),
                           GetFileAttributesA(file_name.c_str()) & ~FILE_ATTRIBUTE_READONLY));
}

#endif

}

TEST_F(file_writer_test, error)
{
    chucho::file::create_directory(file_name_);
    get_writer();
    EXPECT_EQ(1, chucho::status_manager::get().get_count());
    chucho::file::remove(file_name_);
}

TEST_F(file_writer_test, open)
{
    auto w = get_writer();
    EXPECT_EQ(0, chucho::status_manager::get().get_count());
    EXPECT_EQ(file_name_, w->get_file_name());
    EXPECT_EQ(file_name_, w->get_initial_file_name());
}

TEST_F(file_writer_test, truncate)
{
    std::ofstream stream(file_name_.c_str());
    stream << "hello";
    stream.close();
    auto w = get_writer();
    ASSERT_EQ(0, chucho::status_manager::get().get_count());
    w.reset();
    EXPECT_EQ(5, chucho::file::size(file_name_));
    w = get_writer(chucho::file_writer::on_start::TRUNCATE);
    ASSERT_EQ(0, chucho::status_manager::get().get_count());
    w.reset();
    EXPECT_EQ(0, chucho::file::size(file_name_));
}

TEST_F(file_writer_test, write)
{
    auto w = get_writer();
    EXPECT_EQ(0, chucho::status_manager::get().get_count());
    std::shared_ptr<chucho::logger> log = chucho::logger::get("file_writer_test");
    chucho::event evt(log, chucho::level::INFO_(), "hello", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    evt = chucho::event(log, chucho::level::INFO_(), "goodbye", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    w.reset();
    std::ifstream stream(file_name_.c_str());
    std::string line;
    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    EXPECT_STREQ("hello", line.c_str());
    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    EXPECT_STREQ("goodbye", line.c_str());
    std::getline(stream, line);
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    EXPECT_TRUE(stream.eof());
}

TEST_F(file_writer_test, writeable_non_writeable)
{
    auto w = get_writer();
    ASSERT_EQ(0, chucho::status_manager::get().get_count());
    std::shared_ptr<chucho::logger> log = chucho::logger::get("file_writer_test");
    chucho::event evt(log, chucho::level::INFO_(), "hello", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    auto sz = chucho::file::size(file_name_);
    make_unwriteable(file_name_);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    w->write(evt);
    EXPECT_EQ(2, chucho::status_manager::get().get_count());
    EXPECT_EQ(sz, chucho::file::size(file_name_));
    make_writeable(file_name_);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    w->write(evt);
    EXPECT_EQ(4, chucho::status_manager::get().get_count());
    EXPECT_GT(chucho::file::size(file_name_), sz);
}

#if defined(CHUCHO_POSIX)

TEST_F(file_writer_test, writeable_removed)
{
    auto w = get_writer();
    ASSERT_EQ(0, chucho::status_manager::get().get_count());
    std::shared_ptr<chucho::logger> log = chucho::logger::get("file_writer_test");
    chucho::event evt(log, chucho::level::INFO_(), "hello", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    auto sz = chucho::file::size(file_name_);
    chucho::file::remove(file_name_);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    w->write(evt);
    EXPECT_EQ(sz, chucho::file::size(file_name_));
}

#endif
