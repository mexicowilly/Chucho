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

#include <chucho/zeromq_writer.hpp>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/configuration.hpp>
#include <chucho/noop_compressor.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    if (argc != 5 && argc != 6)
    {
        std::cout << "ZeroMQ helper requires four arguments: endpoint, topic, count and message" << std::endl;
        std::cout << "It also allows an optional compressor name" << std::endl;
        return EXIT_FAILURE;
    }
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    std::string topic(argv[2]);
    std::vector<std::uint8_t> pfx(topic.begin(), topic.end());
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    auto ser = std::make_unique<chucho::formatted_message_serializer>();
    std::unique_ptr<chucho::compressor> cmp;
    if (argc == 6)
    {
        if (std::strcmp(argv[5], "noop") == 0)
            cmp = std::make_unique<chucho::noop_compressor>();
    }
    std::unique_ptr<chucho::zeromq_writer> wrt;
    if (cmp)
    {
        wrt = std::make_unique<chucho::zeromq_writer>("zero",
                                                      std::move(fmt),
                                                      std::move(ser),
                                                      std::move(cmp),
                                                      argv[1],
                                                      pfx);
    }
    else
    {
        wrt = std::make_unique<chucho::zeromq_writer>("zero",
                                                      std::move(fmt),
                                                      std::move(ser),
                                                      argv[1],
                                                      pfx);
    }
    chucho::event evt(chucho::logger::get("zeromq_writer_test"),
                      chucho::level::INFO_(),
                      argv[4],
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    // Wait for the client to establish connection
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto count = std::stoul(argv[3]);
    for (decltype(count) i = 0; i < count; i++)
        wrt->write(evt);
    wrt->flush();
    // Wait for client to receive message
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return EXIT_SUCCESS;
}
