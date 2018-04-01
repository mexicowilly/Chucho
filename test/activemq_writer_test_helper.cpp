/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/activemq_writer.hpp>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/configuration.hpp>
#include <chucho/noop_compressor.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 4 && argc != 5)
    {
        std::cout << "ActiveMQ helper requires three arguments: broker, topic and message" << std::endl;
        std::cout << "It also allows an optional compressor name" << std::endl;
        return EXIT_FAILURE;
    }
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    auto ser = std::make_unique<chucho::formatted_message_serializer>();
    std::unique_ptr<chucho::compressor> cmp;
    if (argc == 5)
    {
        if (std::strcmp(argv[4], "noop") == 0)
            cmp = std::make_unique<chucho::noop_compressor>();
    }
    std::unique_ptr<chucho::activemq_writer> wrt;
    if (cmp)
    {
        wrt = std::make_unique<chucho::activemq_writer>("amq",
                                                        std::move(fmt),
                                                        std::move(ser),
                                                        std::move(cmp),
                                                        argv[1],
                                                        chucho::activemq_writer::consumer_type::TOPIC,
                                                        argv[2]);
    }
    else
    {
        wrt = std::make_unique<chucho::activemq_writer>("amq",
                                                        std::move(fmt),
                                                        std::move(ser),
                                                        argv[1],
                                                        chucho::activemq_writer::consumer_type::TOPIC, argv[2]);
    }
    chucho::event evt(chucho::logger::get("activemq_writer_test"),
                      chucho::level::INFO_(),
                      argv[3],
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    wrt->write(evt);
    return EXIT_SUCCESS;
}
