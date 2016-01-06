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
#include <chucho/oracle_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/environment.hpp>
#include <iostream>

TEST(oracle_wrtier_test, oracle)
{
    chucho::logger::remove_unused_loggers();
    auto user = chucho::environment::get("ORACLE_USER");
    auto pass = chucho::environment::get("ORACLE_PASSWORD");
    auto db = chucho::environment::get("ORACLE_DATABASE");
    if (user && pass && db) 
    {
        auto wrt = std::make_shared<chucho::oracle_writer>(std::make_shared<chucho::pattern_formatter>("%m"),
                                                           *user,
                                                           *pass,
                                                           *db);
        auto log = chucho::logger::get("oracle_writer_test");
        log->add_writer(wrt);
        chucho::event evt(log, chucho::level::ERROR_(), "chucho oracle_writer test no mark", __FILE__, __LINE__, __FUNCTION__);
        wrt->write(evt);
        evt = chucho::event(log, chucho::level::INFO_(), "chucho oracle_writer test with mark", __FILE__, __LINE__, __FUNCTION__, "marky");
        wrt->write(evt);
    }
    else
    {
        std::cout << "The environment variables ORACLE_USER, ORACLE_PASSWORD and ORACLE_DATABASE must all be set for this test to run." << std::endl;
    }
}
