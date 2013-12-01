/*
 * Copyright 2013 Will Mason
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
#include <chucho/windows_event_log_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include <chucho/environment.hpp>
#include <iostream>

namespace
{

chucho::event create_event(std::shared_ptr<chucho::logger> log,
                           std::shared_ptr<chucho::level> lvl,
                           const std::string& msg)
{
    return chucho::event(log,
                         lvl,
                         msg,
                         __FILE__,
                         __LINE__,
                         __FUNCTION__);
}

}

TEST(windows_event_log, same_host)
{
    chucho::logger::remove_unused_loggers();
    auto wrt = std::make_shared<chucho::windows_event_log_writer>(
        std::make_shared<chucho::pattern_formatter>("%m"),
        "Chucho_test");
    auto log = chucho::logger::get("windows_event_log_writer_test");
    log->add_writer(wrt);
    wrt->write(create_event(log, chucho::level::INFO_(), "same host INFO"));
    std::cout << "Check your syslog for an error level message \"same host INFO\"" << std::endl;
}
