/*
 * Copyright 2013-2020 Will Mason
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
        auto f = std::make_unique<chucho::pattern_formatter>("%m%n");
        auto w = std::make_unique<chucho::cout_writer>("chucho::cout_writer", std::move(f));
        get_logger()->add_writer(std::move(w));
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
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
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
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
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
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
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
    auto wrts = get_logger()->get_writer_names();
    EXPECT_EQ(0, wrts.size());
}

#endif
