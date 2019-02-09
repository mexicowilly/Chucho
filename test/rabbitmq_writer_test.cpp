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
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <thread>

namespace
{

const char* HOST = "hyena.rmq.cloudamqp.com";
const int PORT = 5672;
const char* VHOST = "tjpxhjkc";
const char* USER = "tjpxhjkc";
const char* PASSWORD = "U51Ue5F_w70sGV945992OmA51WAdT-gs";
const char* URL = "amqp://tjpxhjkc:U51Ue5F_w70sGV945992OmA51WAdT-gs@hyena.rmq.cloudamqp.com/tjpxhjkc";
const amqp_channel_t CHANNEL = 1;
const char* QUEUE = "chucho";
const std::string EXCHANGE("logs");


class rabbitmq_writer_test : public ::testing::Test
{
public:
    void read(const std::string& exp)
    {
        amqp_envelope_t env;
        auto rep = amqp_consume_message(cxn_, &env, nullptr, 0);
        if (rep.reply_type != AMQP_RESPONSE_NORMAL)
            throw chucho::exception("Unable to receive message");
        std::string msg(reinterpret_cast<char*>(env.message.body.bytes), env.message.body.len);
        amqp_destroy_envelope(&env);
        EXPECT_STREQ(exp.c_str(), msg.c_str());
    }

    virtual void SetUp() override
    {
        helper_ = std::string("test") + chucho::file::dir_sep + "rabbitmq-writer-test-helper";
        #if defined(CHUCHO_WINDOWS)
        helper_ += ".exe";
        #endif
        ASSERT_TRUE(chucho::file::exists(helper_));
        cxn_ = amqp_new_connection();
        if (cxn_ == 0)
            throw chucho::exception("Unable to create connection");
        try
        {
            amqp_socket_t* sock = amqp_tcp_socket_new(cxn_);
            if (sock == nullptr)
                throw chucho::exception("Unable to create socket");
            int rc = amqp_socket_open(sock, HOST, PORT);
            if (rc != AMQP_STATUS_OK)
                throw chucho::exception(std::string("Error opening socket: ") + amqp_error_string2(rc));
            amqp_rpc_reply_t rep = amqp_login(cxn_,
                                              VHOST,
                                              0,
                                              AMQP_DEFAULT_FRAME_SIZE,
                                              0,
                                              AMQP_SASL_METHOD_PLAIN,
                                              USER,
                                              PASSWORD);
            if (rep.reply_type != AMQP_RESPONSE_NORMAL)
                throw chucho::exception("Unable to login");
            amqp_channel_open(cxn_, CHANNEL);
            rep = amqp_get_rpc_reply(cxn_);
            if (rep.reply_type != AMQP_RESPONSE_NORMAL)
                throw chucho::exception("Unable to open channel");
            amqp_basic_consume(cxn_,
                               CHANNEL,
                               amqp_cstring_bytes(QUEUE),
                               amqp_empty_bytes,
                               0,
                               1,
                               0,
                               amqp_empty_table);
            rep = amqp_get_rpc_reply(cxn_);
            if (rep.reply_type != AMQP_RESPONSE_NORMAL)
                throw chucho::exception("Unable to declare queue");
        }
        catch (...)
        {
            amqp_channel_close(cxn_, CHANNEL, AMQP_REPLY_SUCCESS);
            amqp_connection_close(cxn_, AMQP_REPLY_SUCCESS);
            amqp_destroy_connection(cxn_);
            throw;
        }
    }

    virtual void TearDown() override
    {
        amqp_channel_close(cxn_, CHANNEL, AMQP_REPLY_SUCCESS);
        amqp_connection_close(cxn_, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(cxn_);
    }

    void write(const std::string& msg)
    {
        std::string cmd = helper_ + ' ' + URL + ' ' + EXCHANGE + ' ' + msg;
        int rc = std::system(cmd.c_str());
        EXPECT_EQ(0, rc);
    }

private:
    std::string helper_;
    amqp_connection_state_t cxn_;
};

}

TEST_F(rabbitmq_writer_test, simple)
{
    std::string msg("MonkeyBalls");
    std::thread thr([&, this] () { read(msg); });
    write(msg);
    thr.join();
}
