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
#include <chucho/logger.hpp>
#include <chucho/email_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/level_threshold_email_trigger.hpp>
#include <chucho/environment.hpp>
#include <chucho/text_util.hpp>

namespace
{

class email_writer_test : public ::testing::Test
{
public:
    virtual void SetUp() override
    {
        chucho::logger::remove_unused_loggers();
        auto host = chucho::environment::get("CHUCHO_EMAIL_HOST");
        ASSERT_TRUE(host);
        auto port_text = chucho::environment::get("CHUCHO_EMAIL_PORT");
        ASSERT_TRUE(port_text);
        std::uint16_t port = std::stoul(*port_text);
        auto from = chucho::environment::get("CHUCHO_EMAIL_FROM");
        ASSERT_TRUE(from);
        auto to_text = chucho::environment::get("CHUCHO_EMAIL_TO");
        ASSERT_TRUE(to_text);
        auto to = chucho::text_util::tokenize(*to_text, ',');
        auto connect_text = chucho::environment::get("CHUCHO_EMAIL_TYPE");
        ASSERT_TRUE(connect_text);
        auto user = chucho::environment::get("CHUCHO_EMAIL_USER");
        ASSERT_TRUE(user);
        auto password = chucho::environment::get("CHUCHO_EMAIL_PASSWORD");
        ASSERT_TRUE(password);
        std::string clow = chucho::text_util::to_lower(*connect_text);
        chucho::email_writer::connection_type connect = chucho::email_writer::connection_type::CLEAR;
        if (*connect_text == "ssl")
            connect = chucho::email_writer::connection_type::SSL;
        else if (*connect_text == "starttls")
            connect = chucho::email_writer::connection_type::STARTTLS;
        auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
        auto trig = std::make_unique<chucho::level_threshold_email_trigger>(chucho::level::ERROR_());
        auto wrt = std::make_unique<chucho::email_writer>("email",
                                                          std::move(fmt),
                                                          *host,
                                                          connect,
                                                          to,
                                                          *from,
                                                          "%c",
                                                          std::move(trig),
                                                          *user,
                                                          *password,
                                                          port);
        get_logger()->add_writer(std::move(wrt));
        wrt_ = dynamic_cast<chucho::email_writer*>(&get_logger()->get_writer("email"));
        ASSERT_NE(nullptr, wrt_);
    }

    std::shared_ptr<chucho::logger> get_logger()
    {
        return chucho::logger::get("email.writer.logger");
    }

protected:
    chucho::email_writer* wrt_;
};

}

TEST_F(email_writer_test, full_queue)
{
    for (int i = 1; i <= 258; i++)
    {
        chucho::event evt(get_logger(),
                          chucho::level::INFO_(), std::string("Message ") + std::to_string(i),
                          __FILE__,
                          __LINE__,
                          __FUNCTION__);
        wrt_->write(evt);
        ASSERT_LE(wrt_->get_buffer_size(), wrt_->get_buffer_capacity());
    }
    chucho::event evt(get_logger(), chucho::level::ERROR_(), "The last", __FILE__, __LINE__, __FUNCTION__);
    wrt_->write(evt);
}

TEST_F(email_writer_test, send_batch)
{
    for (int i = 1; i <= 10; i++)
    {
        chucho::event evt(get_logger(),
                          chucho::level::INFO_(), std::string("Message ") + std::to_string(i),
                          __FILE__,
                          __LINE__,
                          __FUNCTION__);
        wrt_->write(evt);
    }
    chucho::event evt(get_logger(), chucho::level::ERROR_(), "The last", __FILE__, __LINE__, __FUNCTION__);
    wrt_->write(evt);
}

TEST_F(email_writer_test, send_one)
{
    chucho::event evt(get_logger(), chucho::level::ERROR_(), "Hi. This is the email writer.", __FILE__, __LINE__, __FUNCTION__);
    wrt_->write(evt);
}
