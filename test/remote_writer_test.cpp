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

#include <chucho/log.hpp>
#include <chucho/remote_writer.hpp>
#include <chucho/configuration.hpp>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <sstream>
#include <iomanip>

namespace
{

struct options
{
    std::string log_name;
    std::size_t messages_per_second;
    std::size_t total_minutes;
    std::string host_name;
};

// too bad getopt() is POSIX only...
options get_options(int argc, char* argv[])
{
    options opts;
    opts.messages_per_second = 1;
    opts.total_minutes = 1;
    opts.host_name = "localhost";
    for (int i = 1; i < argc; i++)
    {
        std::string cur(argv[i]);
        if (cur == "-l" || cur == "--log")
        {
            if (++i == argc)
            {
                std::cout << "The log argument is missing" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            opts.log_name = argv[i];
        }
        else if (cur == "-v" || cur == "--messages_per_second")
        {
            if (++i == argc)
            {
                std::cout << "The messages per second argument is missing" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            opts.messages_per_second = std::stoul(argv[i]);
        }
        else if (cur == "-m" || cur == "--total_minutes")
        {
            if (++i == argc)
            {
                std::cout << "The total minutes argument is missing" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            opts.total_minutes = std::stoul(argv[i]);
        }
        else if (cur == "-h" || cur == "--host")
        {
            if (++i == argc)
            {
                std::cout << "The host argument is missing" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            opts.host_name = argv[i];
        }
        else
        {
            std::cout << "Unknown argument " << cur << std::endl;
        }
    }
    if (opts.log_name.empty())
    {
        std::cout << "The log argument is required" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return opts;
}

std::string next_message()
{
    static std::size_t index = 0;

    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(8) << index++;
    return stream.str();
}

}

int main(int argc, char* argv[])
{
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    options opts = get_options(argc, argv);
    std::shared_ptr<chucho::logger> lgr(chucho::logger::get(opts.log_name));
    lgr->add_writer(std::make_shared<chucho::remote_writer>(opts.host_name));
    std::chrono::nanoseconds pause(1000000000 / opts.messages_per_second);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() +
        std::chrono::minutes(opts.total_minutes);
    while (std::chrono::steady_clock::now() < end)
    {
        std::string msg(next_message());
        CHUCHO_INFO(lgr, msg);
        std::cout << msg << std::endl;
        std::this_thread::sleep_for(pause);
    }
    return EXIT_SUCCESS;
}
