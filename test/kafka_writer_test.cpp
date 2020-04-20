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
#include <librdkafka/rdkafka.h>
#include <chucho/file.hpp>
#include <thread>
#include <iostream>

namespace
{

const char* BROKER = "192.168.56.101";
const char* TOPIC = "chucho";

class kafka_writer_test : public ::testing::Test
{
public:
    void read(const std::string& exp)
    {
        auto start = std::chrono::steady_clock::now();
        std::vector<std::string> msgs;
        int i;
        std::cout << "Time 0" << std::endl;
        for (i = 0; i < 9; i++)
        {
            auto msg = rd_kafka_consumer_poll(consumer_, 500);
            if (msg != nullptr)
            {
                if (msg->err == 0)
                {
                    std::cout << "Message " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
                    msgs.push_back(std::string(reinterpret_cast<char *>(msg->payload), msg->len));
                }
                else
                {
                    std::cout << "Consumer error: " << rd_kafka_message_errstr(msg);
                }
                rd_kafka_message_destroy(msg);
            }
        }
        if (msgs.empty())
            FAIL() << "Too many tries to read from Kafka server";
        else
            EXPECT_STREQ(exp.c_str(), msgs.back().c_str());
    }

    virtual void SetUp() override
    {
        helper_ = std::string("test") + chucho::file::dir_sep + "kafka-writer-test-helper";
#if defined(CHUCHO_WINDOWS)
        helper_ += ".exe";
#endif
        ASSERT_TRUE(chucho::file::exists(helper_));
        char err_msg[1024];
        auto conf = rd_kafka_conf_new();
        auto rc = rd_kafka_conf_set(conf, "bootstrap.servers", BROKER, err_msg, sizeof(err_msg));
        ASSERT_EQ(RD_KAFKA_CONF_OK, rc) << err_msg;
        rc = rd_kafka_conf_set(conf, "group.id", "test", err_msg, sizeof(err_msg));
        ASSERT_EQ(RD_KAFKA_CONF_OK, rc) << err_msg;
        rc = rd_kafka_conf_set(conf, "auto.offset.reset", "earliest", err_msg, sizeof(err_msg));
        ASSERT_EQ(RD_KAFKA_CONF_OK, rc) << err_msg;
        consumer_ = rd_kafka_new(RD_KAFKA_CONSUMER, conf, err_msg, sizeof(err_msg));
        ASSERT_NE(nullptr, consumer_) << err_msg;
        rd_kafka_poll_set_consumer(consumer_);
        auto tpc = rd_kafka_topic_partition_list_new(1);
        rd_kafka_topic_partition_list_add(tpc, TOPIC, RD_KAFKA_PARTITION_UA);
        auto rc2 = rd_kafka_subscribe(consumer_, tpc);
        ASSERT_EQ(0, rc2) << rd_kafka_err2str(rc2);
        rd_kafka_topic_partition_list_destroy(tpc);
    }

    virtual void TearDown() override
    {
        rd_kafka_destroy(consumer_);
    }

    void write(const std::string& msg)
    {
        std::string cmd = helper_ + ' ' + BROKER + ' ' + TOPIC + ' ' + msg;
        int rc = std::system(cmd.c_str());
        EXPECT_EQ(0, rc);
    }

private:
    std::string helper_;
    rd_kafka_t* consumer_;
};

}

TEST_F(kafka_writer_test, simple)
{
    std::string msg("MonkeyBalls");
    std::thread thr([&, this] () { read(msg); });
    write(msg);
    thr.join();
}

