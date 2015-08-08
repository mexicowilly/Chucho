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
#include <chucho/logger.hpp>
#include <chucho/email_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/error_level_email_trigger.hpp>

TEST(email_writer_test, send_one)
{
    chucho::logger::remove_unused_loggers();
    std::vector<std::string> to;
    to.push_back("mexicowilly@mac.com");
    auto wrt = std::make_shared<chucho::email_writer>(std::make_shared<chucho::pattern_formatter>("%m%n"),
                                                      "smtp.gmail.com",
                                                      chucho::email_writer::connection_type::STARTTLS,
                                                      to,
                                                      "willchido@gmail.com",
                                                      "%c",
                                                      std::make_shared<chucho::error_level_email_trigger>(),
                                                      "willchido@gmail.com",
                                                      "ValeVerga",
                                                      587);
    auto log = chucho::logger::get("email.writer.logger");
    log->add_writer(wrt);
    chucho::event evt(log, chucho::level::ERROR_(), "Hi. This is the email writer.", __FILE__, __LINE__, __FUNCTION__);
    wrt->write(evt);
}