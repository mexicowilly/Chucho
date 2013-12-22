/*
 * Copyright 2013-2014 Will Mason
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
#include <chucho/syslog_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include <chucho/environment.hpp>
#include <iostream>

#if !defined(CHUCHO_WINDOWS)

TEST(syslog_wrtier_test, same_host)
{
    chucho::logger::remove_unused_loggers();
    auto wrt = std::make_shared<chucho::syslog_writer>(std::make_shared<chucho::pattern_formatter>("%m"),
                                                       chucho::syslog::facility::LOCAL0);
    auto log = chucho::logger::get("syslog_writer_test");
    log->add_writer(wrt);
    chucho::event evt(log, chucho::level::ERROR_(), "chucho syslog_writer test same host", __FILE__, __LINE__, __FUNCTION__);
    wrt->write(evt);
    std::cout << "Check your syslog for an error level message \"chucho syslog_writer test same host\"" << std::endl;
}

#endif

TEST(syslog_writer_test, remote_host)
{
    chucho::logger::remove_unused_loggers();
    chucho::optional<std::string> env = chucho::environment::get("CHUCHO_SYSLOG_HOST");
    std::string host = env ? *env : chucho::host::get_full_name();
    auto wrt = std::make_shared<chucho::syslog_writer>(std::make_shared<chucho::pattern_formatter>("%m"),
                                                       chucho::syslog::facility::LOCAL0,
                                                       host);
    auto log = chucho::logger::get("syslog_writer_test");
    log->add_writer(wrt);
    chucho::event evt(log, chucho::level::ERROR_(), "chucho syslog_writer test remote host", __FILE__, __LINE__, __FUNCTION__);
    wrt->write(evt);
    std::cout << "Check your syslog for an error level message \"chucho syslog_writer test remote host\"" << std::endl;
}
