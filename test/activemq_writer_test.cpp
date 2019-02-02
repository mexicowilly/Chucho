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
#include <activemq/library/ActiveMQCPP.h>
#include <cms/ConnectionFactory.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <cms/Connection.h>
#include <cms/CMSException.h>
#include <cstring>
#include <thread>

namespace
{

const std::string BROKER("tcp://127.0.0.1:61616");
const std::string TOPIC("Chucho");

class activemq_writer_test : public ::testing::Test,
                             public cms::ExceptionListener
{
public:
    virtual void onException(const cms::CMSException& e) override
    {
        FAIL() << e.getMessage();
    }

    void read(const std::string& exp)
    {
        std::unique_ptr<cms::Message> msg(consumer_->receive());
        cms::BytesMessage* bmsg = dynamic_cast<cms::BytesMessage*>(msg.get());
        ASSERT_NE(nullptr, bmsg);
        ASSERT_EQ(exp.length(), bmsg->getBodyLength());
        auto bytes = bmsg->getBodyBytes();
        EXPECT_EQ(0, std::memcmp(bytes, exp.data(), exp.length()));
        delete [] bytes;
    }

    virtual void SetUp() override
    {
        helper_ = std::string("test") + chucho::file::dir_sep + "activemq-writer-test-helper";
        #if defined(CHUCHO_WINDOWS)
        helper_ += ".exe";
        #endif
        if (chucho::file::exists(helper_))
        {
            activemq::library::ActiveMQCPP::initializeLibrary();
            std::unique_ptr<cms::ConnectionFactory> fact(cms::ConnectionFactory::createCMSConnectionFactory(BROKER));
            connection_ = fact->createConnection();
            connection_->start();
            connection_->setExceptionListener(this);
            session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
            destination_ = session_->createTopic(TOPIC);
            consumer_ = session_->createConsumer(destination_);
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
            connection_->close();
            delete consumer_;
            delete destination_;
            delete session_;
            delete connection_;
            activemq::library::ActiveMQCPP::shutdownLibrary();
        }
    }

    void write(const std::string& msg, const std::string& cmp = std::string())
    {
        std::string cmd = helper_ + ' ' + BROKER + ' ' + TOPIC + ' ' + msg;
        if (!cmp.empty())
            cmd += ' ' + cmp;
        int rc = std::system(cmd.c_str());
        EXPECT_EQ(0, rc);
    }

protected:
    std::string helper_;

private:
    cms::Connection* connection_;
    cms::Session* session_;
    cms::Destination* destination_;
    cms::MessageConsumer* consumer_;
};

}

TEST_F(activemq_writer_test, compress)
{
    if (helper_.empty())
    {
        FAIL() << "This test can only run with a make file CMake generator";
    }
    else
    {
        std::string msg("SquozenMonkeyBalls");
        std::thread thr([&, this] () { read(msg); });
        write(msg, "noop");
        thr.join();
    }
}

TEST_F(activemq_writer_test, simple)
{
    if (helper_.empty())
    {
        FAIL() << "This test can only run with a make file CMake generator";
    }
    else
    {
        std::string msg("MonkeyBalls");
        std::thread thr([&, this] () { read(msg); });
        write(msg);
        thr.join();
    }
}
