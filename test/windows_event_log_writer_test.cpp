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
#include <chucho/windows_event_log_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <array>

namespace
{

chucho::event create_event(std::shared_ptr<chucho::level> lvl,
                           const std::string& msg)
{
    return chucho::event(chucho::logger::get("windows_event_writer_test"),
                         lvl,
                         msg,
                         __FILE__,
                         __LINE__,
                         __FUNCTION__);
}

class windows_event_log_writer_test : public ::testing::Test
{
public:
    windows_event_log_writer_test()
    {
        SetEnvironmentVariableA("CHUCHO_EVENT_LOG_DLL", CHUCHO_EVENT_LOG_DLL);
        chucho::logger::remove_unused_loggers();
        wrt_ = std::make_shared<chucho::windows_event_log_writer>(
            std::make_shared<chucho::pattern_formatter>("%m"),
            "ChuchoTest");
    }

protected:
    std::shared_ptr<chucho::writer> wrt_;
};

}

TEST_F(windows_event_log_writer_test, custom_log)
{
    wrt_ = std::make_shared<chucho::windows_event_log_writer>(
        std::make_shared<chucho::pattern_formatter>("%m"),
        "ChuchoLog",
        "ChuchoLogTest");
    wrt_->write(create_event(chucho::level::INFO_(), "custom log"));
}

TEST_F(windows_event_log_writer_test, levels)
{
    std::array<std::shared_ptr<chucho::level>, 6> lvls =
    {
        chucho::level::TRACE_(),
        chucho::level::DEBUG_(),
        chucho::level::INFO_(),
        chucho::level::WARN_(),
        chucho::level::ERROR_(),
        chucho::level::FATAL_()
    };
    for (auto lvl : lvls)
        wrt_->write(create_event(lvl, lvl->get_name()));
}

TEST_F(windows_event_log_writer_test, same_host)
{
    wrt_->write(create_event(chucho::level::INFO_(), "same host INFO"));
}
