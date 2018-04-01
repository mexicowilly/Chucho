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
#include <chucho/message_queue_writer.hpp>
#include <chucho/file.hpp>
#include <zmq.h>
#include <thread>
#include <cstring>
#include <sstream>

namespace
{

const std::string ENDPOINT("tcp://127.0.0.1:31491");
const std::string TOPIC("Chucho");

class zeromq_writer_test : public ::testing::Test
{
public:
    void read(std::size_t count, const std::string& exp)
    {
        int rc = zmq_connect(socket_, ENDPOINT.c_str());
        ASSERT_EQ(0, rc);
        int scount = count;
        while (scount > 0)
        {
            rc = zmq_msg_recv(&msg_, socket_, 0);
            ASSERT_EQ(TOPIC.length(), rc);
            EXPECT_EQ(0, std::memcmp(zmq_msg_data(&msg_), TOPIC.data(), TOPIC.length()));
            ASSERT_NE(0, zmq_msg_more(&msg_));
            rc = zmq_msg_recv(&msg_, socket_, 0);
            std::istringstream input(std::string(reinterpret_cast<const char*>(zmq_msg_data(&msg_)), zmq_msg_size(&msg_)));
            for (std::string line; std::getline(input, line); )
                EXPECT_EQ(exp, line);
            EXPECT_EQ(0, zmq_msg_more(&msg_));
            scount -= chucho::message_queue_writer::DEFAULT_COALESCE_MAX;
        }
    }

    virtual void SetUp() override
    {
        helper_ = std::string("test") + chucho::file::dir_sep + "zeromq-writer-test-helper";
        #if defined(CHUCHO_WINDOWS)
        helper_ += ".exe";
        #endif
        if (chucho::file::exists(helper_))
        {
            ctx_ = zmq_ctx_new();
            socket_ = zmq_socket(ctx_, ZMQ_SUB);
            ASSERT_NE(nullptr, socket_);
            int rc = zmq_setsockopt(socket_, ZMQ_SUBSCRIBE, TOPIC.c_str(), TOPIC.length());
            ASSERT_EQ(0, rc);
            zmq_msg_init(&msg_);
        }
        else
        {
            helper_.clear();
        }
    }

    virtual void TearDown() override
    {
        if (!helper_.empty())
        {
            zmq_msg_close(&msg_);
            zmq_close(socket_);
            zmq_ctx_destroy(ctx_);
        }
    }

    void write(std::size_t count, const std::string& msg, const std::string& cmp = std::string())
    {
        std::string cmd = helper_ + ' ' + ENDPOINT + ' ' + TOPIC + ' ' + std::to_string(count) + ' ' + msg;
        if (!cmp.empty())
            cmd += ' ' + cmp;
        int rc = std::system(cmd.c_str());
        EXPECT_EQ(0, rc);
    }

protected:
    std::string helper_;

private:
    void* ctx_;
    void* socket_;
    zmq_msg_t msg_;
};

}

TEST_F(zeromq_writer_test, compress)
{
    if (helper_.empty())
    {
        FAIL() << "This test can only run with a make file CMake generator";
    }
    else
    {
        std::string msg("MonkeyBalls");
        std::thread thr([&, this] () { read(1, msg); });
        write(1, msg, "noop");
        thr.join();
    }
}

TEST_F(zeromq_writer_test, simple)
{
    if (helper_.empty())
    {
        FAIL() << "This test can only run with a make file CMake generator";
    }
    else
    {
        std::string msg("MonkeyBalls");
        std::thread thr([&, this] () { read(50, msg); });
        write(50, msg);
        thr.join();
    }
}
