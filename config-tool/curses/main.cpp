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

#include "controller.hpp"
#include <chucho/file_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/configuration.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace
{

void configure_logging()
{
    chucho::configuration::set_style(chucho::configuration::style::OFF);
    auto fmt = std::make_shared<chucho::pattern_formatter>("%D{%y/%m/%d %H:%M:%S} %p %c %m%n");
    auto wrt = std::make_shared<chucho::file_writer>(fmt,
                                                     "chucho-config.log",
                                                     chucho::file_writer::on_start::TRUNCATE);
    chucho::logger::get("")->add_writer(wrt);
}

}

int main(int argc, char* argv[])
{
    try
    {
        configure_logging();
        chucho::config_tool::properties props(argc, argv);
        chucho::config_tool::controller cntrl(props);
        cntrl.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Unexpected error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
