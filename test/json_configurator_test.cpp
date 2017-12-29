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

#include "configurator_test.hpp"
#include <chucho/json_configurator.hpp>
#include <chucho/configuration.hpp>
#include <chucho/logger.hpp>

namespace
{

class json_configurator : public chucho::test::configurator
{
protected:
    json_configurator();

    virtual chucho::configurator& get_configurator() override;

private:
    chucho::json_configurator cnf_;
};

chucho::configurator& json_configurator::get_configurator()
{
    return cnf_;
}

json_configurator::json_configurator()
    : cnf_(chucho::configuration::get_security_policy())
{
}

}

TEST_F(json_configurator, cerr_writer)
{
    configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "writers" : [{
                "chucho::cerr_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" }
                }
            }]
        }
    }
}
)cnf");
    cerr_writer_body();
}

TEST_F(json_configurator, duplicate_message_filter)
{
    configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "writers" : [{
                "chucho::cout_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" },
                    "chucho::duplicate_message_filter" : {}
                }
            }]
        }
    }
}
)cnf");
    duplicate_message_filter_body();
}

TEST_F(json_configurator, logger)
{
    configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "level" : "fatal",
            "writes_to_ancestors" : false
        }
    }
}
)cnf");
    logger_body();
}

TEST_F(json_configurator, lzma_file_compressor)
{
    configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "writers" : [{
                "chucho::rolling_file_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" },
                    "chucho::numbered_file_roller" : {
                        "min_index" : 3,
                        "max_index" : 5,
                        "chucho::lzma_file_compressor" : { "min_index" : 1 }
                    },
                    "chucho::size_file_roll_trigger" : { "max_size" : "5000B" },
                    "file_name" : "what.log"
                }
            }]
        }
    }
}
)cnf");
    lzma_file_compressor_body();
}

TEST_F(json_configurator, multiple_writer)
{
configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "writers" : [
                { "chucho::file_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" },
                    "file_name" : "one.log",
                    "name" : "one"
                } },
                { "chucho::file_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" },
                    "file_name" : "two.log"
                    "name" : "two"
                } }
            ]
        }
    }
}
)cnf");
    multiple_writer_body();
}

TEST_F(json_configurator, rolling_file_writer)
{
    configure(R"cnf(
{
    "chucho_loggers" : {
        "will" : {
            "writers" : [{
                "chucho::rolling_file_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" },
                    "chucho::numbered_file_roller" : {
                        "min_index" : 3,
                        "max_index" : 5
                    },
                    "chucho::size_file_roll_trigger" : { "max_size" : "5000b" },
                    "file_name" : "what.log",
                    "on_start" : "TruNCAte",
                    "flush" : false
                }
            }]
        }
    }
}
)cnf");
    rolling_file_writer_body();
}

TEST_F(json_configurator, root_alias)
{
    chucho::logger::get("")->clear_writers();
    configure(R"cnf(
{
    "chucho_loggers" : {
        "<root>" : {
            "writers" : [{
                "chucho::cout_writer" : {
                    "chucho::pattern_formatter" : { "pattern" : "%m%n" }
                }
            }]
        }
    }
}
)cnf");
    root_alias_body();
}
