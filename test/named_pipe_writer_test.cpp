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
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <cstdlib>
#include <thread>
#if defined(CHUCHO_POSIX)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace
{

class named_pipe_writer_test : public ::testing::Test
{
public:
    named_pipe_writer_test()
        : file_name_("named_pipe_writer_test"),
          helper_name_(std::string("test") + chucho::file::dir_sep + "named-pipe-writer-test-helper")
    {
        #if defined(CHUCHO_WINDOWS)
        helper_name_ += ".exe";
        #endif
        // If we are using a generator like Visual Studio, then the
        // output could go into a subdirectory, like Debug or something.
        // So, we only let these tests run if the generator is a make
        // file one or one that puts the output where we expect.
        if (!chucho::file::exists(helper_name_))
            helper_name_.clear();
        chucho::file::remove(file_name_);
    }

    ~named_pipe_writer_test()
    {
        chucho::file::remove(file_name_);
    }

    void make_pipe()
    {
        #if defined(CHUCHO_POSIX)
        if (mkfifo(file_name_.c_str(), 0644) != 0)
            throw chucho::exception("Could not create named pipe " + file_name_);
        #endif
    }

    std::string read(std::size_t num)
    {
        read_text_.clear();
        #if defined(CHUCHO_POSIX)
        int fd = ::open(file_name_.c_str(), O_RDONLY);
        EXPECT_GT(fd, 0);
        std::vector<char> buf(num);
        auto num_read = ::read(fd, &buf[0], num);
        EXPECT_EQ(num, num_read);
        ::close(fd);
        read_text_.assign(&buf[0], buf.size());
        #endif
    }

    void write(const std::string& str)
    {
        std::string cmd = helper_name_ + ' ' + file_name_ + ' ' + str;
        int rc;
        rc = std::system(cmd.c_str());
        EXPECT_EQ(0, rc);
    }

protected:
    std::string file_name_;
    std::string helper_name_;
    // This is necessary to work around a bug in g++ 4.9.2 where it crashes
    // when evaluating a lambda with capture of [&, this]
    std::string read_text_;
};

}

TEST_F(named_pipe_writer_test, simple)
{
    if (helper_name_.empty())
    {
        FAIL() << "This test can only run with a make file CMake generator";
    }
    else
    {
        make_pipe();
        std::thread thr1([this] () { read(6); });
        write("Chucho");
        thr1.join();
        EXPECT_STREQ("Chucho", read_text_.c_str());
    }
}