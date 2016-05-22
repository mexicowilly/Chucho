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
#include <chucho/file.hpp>
#include <chucho/door_event.h>
#include <door.h>
#include <fcntl.h>
#include <stropts.h>
#include <ctime>
#include <iostream>

namespace
{

std::string message;
std::string marker;

void door_proc(void*, char* arg, size_t arg_len, door_desc_t* desc, uint_t desc_num)
{
    chucho_door_event* evt = reinterpret_cast<chucho_door_event*>(arg);
    std::cout << "formatted message " << evt->formatted_message_len << " \"" << CHUCHO_DOOR_FORMATTED_MESSAGE(evt) << "\"" << std::endl;
    std::cout << "timestamp " << std::ctime(&evt->timestamp);
    std::cout << "file name " << evt->file_name_len << " \"" << CHUCHO_DOOR_FILE_NAME(evt) << "\"" << std::endl;
    std::cout << "line number " << evt->line_number << std::endl;
    std::cout << "function name " << evt->function_name_len << " \"" << CHUCHO_DOOR_FUNCTION_NAME(evt) << "\"" << std::endl;
    std::cout << "logger name " << evt->logger_name_len << " \"" << CHUCHO_DOOR_LOGGER_NAME(evt) << "\"" << std::endl;
    std::cout << "level name " << evt->level_name_len << " \"" << CHUCHO_DOOR_LEVEL_NAME(evt) << "\"" << std::endl;
    std::cout << "marker " << evt->marker_len;
    if (evt->marker_len > 0)
        std::cout  << " \"" << CHUCHO_DOOR_MARKER(evt) << "\"";
    std::cout << std::endl;
    std::cout << "thread id " << evt->thread_id_len << " \"" << CHUCHO_DOOR_THREAD_ID(evt) << "\"" << std::endl;
    message = CHUCHO_DOOR_FORMATTED_MESSAGE(evt);
    if (evt->marker_len > 0)
        marker = CHUCHO_DOOR_MARKER(evt);
    else
        marker.clear();
    door_return(nullptr, 0, nullptr, 0);
}

class door_writer_test : public ::testing::Test
{
public:
    door_writer_test()
        : door_name_("door_writer_test"),
          helper_name_("test/door-writer-test-helper"),
          door_(-1)
    {
        if (!chucho::file::exists(door_name_))
        {
            int tmp = open(door_name_.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
            if (tmp == -1)
                FAIL() << "Could not create the door file: " << std::strerror(errno);
            close(tmp);
        }
        door_ = door_create(door_proc, nullptr, 0);
        if (door_ == -1)
            FAIL() << "Unable to create the door";
        if (fattach(door_, door_name_.c_str()) != 0)
            FAIL() << "Unable to attach door to file system: " << std::strerror(errno);
    }

    ~door_writer_test()
    {
        close(door_);
        chucho::file::remove(door_name_);
    }

    void write(const std::string& str, const std::string& mrk = std::string())
    {
        std::string cmd = helper_name_ + ' ' + door_name_ + ' ' + str;
        if (!mrk.empty())
            cmd += ' ' + mrk;
        int rc;
        rc = std::system(cmd.c_str());
        ASSERT_EQ(0, rc);
    }

protected:
    std::string door_name_;
    std::string helper_name_;
    int door_;
};

}

TEST_F(door_writer_test, no_marker)
{
    write("monkeyballs");
    EXPECT_STREQ("monkeyballs", message.c_str());
    EXPECT_TRUE(marker.empty());
}

TEST_F(door_writer_test, with_marker)
{
    write("markdoodle", "marky");
    EXPECT_STREQ("markdoodle", message.c_str());
    EXPECT_STREQ("marky", marker.c_str());
}