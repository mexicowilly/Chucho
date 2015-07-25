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
#include <chucho/configuration.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/cerr_writer.hpp>

namespace
{

class configuration : public ::testing::Test
{
public:
    configuration()
    {
        chucho::logger::remove_unused_loggers();
        std::shared_ptr<chucho::formatter> f = std::make_shared<chucho::pattern_formatter>("%m%n");
        std::shared_ptr<chucho::writer> w = std::make_shared<chucho::cout_writer>(f);
        get_logger()->add_writer(w);
    }

    ~configuration()
    {
        chucho::logger::remove_unused_loggers();
    }

protected:
    std::shared_ptr<chucho::logger> get_logger() const
    {
        return chucho::logger::get(get_logger_name());
    }

    const char* get_logger_name() const
    {
        return "one";
    }
};

}

#if defined(CHUCHO_CONFIG_FILE_CONFIG)

TEST_F(configuration, set_config)
{
    std::ostringstream stream;
    stream << "chucho.logger = " << get_logger_name() << std::endl
           << "chucho.logger." << get_logger_name() << ".writer = ce" << std::endl
           << "chucho.writer.ce = chucho::cerr_writer" << std::endl
           << "chucho.writer.ce.formatter = pf" << std::endl
           << "chucho.formatter.pf = chucho::pattern_formatter" << std::endl
           << "chucho.formatter.pf.pattern = %m%n";
    ASSERT_TRUE(chucho::configuration::set(stream.str()));
    auto wrts = get_logger()->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_TRUE(static_cast<bool>(std::dynamic_pointer_cast<chucho::cerr_writer>(wrts[0])));
}

#endif

#if defined(CHUCHO_LOG4CPLUS_CONFIG)

TEST_F(configuration, set_log4cplus)
{
    std::ostringstream stream;
    stream << "log4cplus.logger." << get_logger_name() << " = info, ce\n"
           << "log4cplus.appender.ce = log4cplus::ConsoleAppender\n"
           << "log4cplus.appender.ce.logToStdErr = true\n"
           << "log4cplus.appender.ce.layout = log4cplus::PatternLayout\n"
           << "log4cplus.appender.ce.layout.ConversionPattern = %m%n";
    ASSERT_TRUE(chucho::configuration::set(stream.str()));
    auto wrts = get_logger()->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_TRUE(static_cast<bool>(std::dynamic_pointer_cast<chucho::cerr_writer>(wrts[0])));
}

#endif

#if defined(CHUCHO_YAML_CONFIG)

TEST_F(configuration, set_yaml)
{
    std::ostringstream stream;
    stream << "- chucho::logger:" << std::endl
           << "    name: " << get_logger_name() << std::endl
           << "    chucho::cerr_writer:" << std::endl
           << "        chucho::pattern_formatter:" << std::endl
           << "            pattern: '%m%n'";
    ASSERT_TRUE(chucho::configuration::set(stream.str()));
    auto wrts = get_logger()->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_TRUE(static_cast<bool>(std::dynamic_pointer_cast<chucho::cerr_writer>(wrts[0])));
}

TEST_F(configuration, set_yaml_error)
{
    std::ostringstream stream;
    stream << "- chucho::logger:" << std::endl
           << "    name: " << get_logger_name() << std::endl
           << "    chucho::cerr_writer:" << std::endl
           << "        chucho::monkey_balls:" << std::endl
           << "            pattern: '%m%n'";
    ASSERT_TRUE(chucho::configuration::set(stream.str()));
    auto wrts = get_logger()->get_writers();
    EXPECT_EQ(0, wrts.size());
}

#endif
