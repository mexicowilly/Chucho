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
#include <chucho/sqlite_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <thread>

TEST(sqlite_writer_test, sqlite)
{
    chucho::logger::remove_unused_loggers();
    auto wrt = std::make_unique<chucho::sqlite_writer>("sqlite",
                                                       std::move(std::make_unique<chucho::pattern_formatter>("%m")),
                                                       "sqlite_writer.sqlite");
    auto log = chucho::logger::get("sqlite_writer_test");
    log->add_writer(std::move(wrt));
    chucho::event evt(log, chucho::level::ERROR_(), "chucho sqlite_writer test no mark", __FILE__, __LINE__, __FUNCTION__);
    auto& sql = dynamic_cast<chucho::sqlite_writer&>(log->get_writer("sqlite"));
    sql.write(evt);
    evt = chucho::event(log, chucho::level::INFO_(), "chucho sqlite_writer test with mark", __FILE__, __LINE__, __FUNCTION__, "marky");
    sql.write(evt);
}
