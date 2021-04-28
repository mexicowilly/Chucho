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
#include <chucho/configuration.hpp>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/regex.hpp>

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

    bool set_config(const char* const tmpl)
    {
        chucho::regex::expression re("LGR");
        auto cfg = chucho::regex::replace(tmpl, re, get_logger_name());
        return chucho::configuration::set(cfg);
    }
};

}

TEST_F(configuration, set_config)
{
    const char* tmpl = R"cfg(
chucho.logger = LGR
chucho.logger.LGR.writer = ce
chucho.writer.ce = chucho::cerr_writer
chucho.writer.ce.formatter = pf
chucho.formatter.pf = chucho::pattern_formatter
chucho.formatter.pf.pattern = %m%n
)cfg";
    ASSERT_TRUE(set_config(tmpl));
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
}

TEST_F(configuration, set_invalid)
{
    const char* tmpl = R"cfg(
- chucho::logger:
 name: LGR
  chucho::cerr_writer:
    chucho::pattern_formatter:
      pattern: '%m%n'
)cfg";
    ASSERT_FALSE(set_config(tmpl));
}

TEST_F(configuration, set_json)
{
    const char* tmpl = R"cfg(
{
    "chucho_loggers" : {
        "LGR" : {
            "writers" : [{
                "chucho::cerr_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" }
                }
            }]
        }
    }
}
)cfg";
    ASSERT_TRUE(set_config(tmpl));
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
}

TEST_F(configuration, set_log4cplus)
{
    const char* tmpl = R"cfg(
log4cplus.logger.LGR = info, ce
log4cplus.appender.ce = log4cplus::ConsoleAppender
log4cplus.appender.ce.logToStdErr = true
log4cplus.appender.ce.layout = log4cplus::PatternLayout
log4cplus.appender.ce.layout.ConversionPattern = %m%n
)cfg";
    ASSERT_TRUE(set_config(tmpl));
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
}

TEST_F(configuration, set_yaml)
{
    const char* tmpl = R"cfg(
- chucho::logger:
    name: LGR
    chucho::cerr_writer:
        chucho::pattern_formatter:
            pattern: '%m%n'
)cfg";
    ASSERT_TRUE(set_config(tmpl));
    EXPECT_NO_THROW(get_logger()->get_writer("chucho::cerr_writer"));
}

TEST_F(configuration, set_yaml_error)
{
    const char* tmpl = R"cfg(
- chucho::logger:
    name: LGR
    chucho::cerr_writer:
        chucho::monkey_balls:
            pattern: '%m%n'
)cfg";
    ASSERT_TRUE(set_config(tmpl));
    auto wrts = get_logger()->get_writer_names();
    EXPECT_EQ(0, wrts.size());
}
