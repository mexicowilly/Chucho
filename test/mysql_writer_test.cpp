/*
 * Copyright 2013-2017 Will Mason
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
#include <chucho/mysql_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/environment.hpp>
#include <iostream>
#include <thread>

TEST(mysql_wrtier_test, mysql)
{
    chucho::logger::remove_unused_loggers();
    auto host = chucho::environment::get("MYSQL_HOST");     // 192.168.56.101
    auto user = chucho::environment::get("MYSQL_USER");     // will
    auto pass = chucho::environment::get("MYSQL_PASSWORD"); // yavasya
    auto db = chucho::environment::get("MYSQL_DATABASE");   // test
    if (host && user && pass && db) 
    {
        auto wrt = std::make_shared<chucho::mysql_writer>(std::make_shared<chucho::pattern_formatter>("%m"),
                                                          *host,
                                                          *user,
                                                          *pass,
                                                          *db);
        auto log = chucho::logger::get("mysql_writer_test");
        log->add_writer(wrt);
        chucho::event evt(log, chucho::level::ERROR_(), "chucho mysql_writer test no mark", __FILE__, __LINE__, __FUNCTION__);
        wrt->write(evt);
        evt = chucho::event(log, chucho::level::INFO_(), "chucho mysql_writer test with mark", __FILE__, __LINE__, __FUNCTION__, "marky");
        wrt->write(evt);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    else
    {
        std::cout << "The environment variables MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD and MYSQL_DATABASE must all be set for this test to run." << std::endl;
    }
}
