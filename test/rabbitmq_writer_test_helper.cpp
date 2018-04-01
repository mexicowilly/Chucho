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

#include <chucho/rabbitmq_writer.hpp>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/configuration.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "RabbitMQ helper requires three arguments: URL, exchange and message" << std::endl;
        return EXIT_FAILURE;
    }
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    auto ser = std::make_unique<chucho::formatted_message_serializer>();
    try
    {
        auto wrt = std::make_unique<chucho::rabbitmq_writer>("rabbit",
                                                             std::move(fmt),
                                                             std::move(ser),
                                                             argv[1],
                                                             argv[2]);
        chucho::event evt(chucho::logger::get("rabbitmq_writer_test"),
                          chucho::level::INFO_(),
                          argv[3],
                          __FILE__,
                          __LINE__,
                          __FUNCTION__);
        wrt->write(evt);
    }
    catch (std::exception& e)
    {
        std::cout << "Helper error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
