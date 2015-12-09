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

#include <chucho/zeromq_writer.hpp>
#include <chucho/formatted_message_serializer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "ZeroMQ helper requires three arguments: endpoint, topic and message" << std::endl;
        return EXIT_FAILURE;
    }
    std::string topic(argv[2]);
    std::vector<std::uint8_t> pfx(topic.begin(), topic.end());
    auto fmt = std::make_shared<chucho::pattern_formatter>("%m");
    auto ser = std::make_shared<chucho::formatted_message_serializer>();
    chucho::zeromq_writer wrt(fmt, ser, argv[1], pfx);
    chucho::event evt(chucho::logger::get("zeromq_writer_test"),
                      chucho::level::INFO_(),
                      argv[3],
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    wrt.write(evt);
    return EXIT_SUCCESS;
}