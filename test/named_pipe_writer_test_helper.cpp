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

#include <chucho/named_pipe_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <cstdlib>

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
            return EXIT_FAILURE;
        auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
        auto wrt = std::make_unique<chucho::named_pipe_writer>("named", std::move(fmt), argv[1]);
        std::shared_ptr<chucho::logger> log = chucho::logger::get("named_pipe_writer_test");
        chucho::event evt(log, chucho::level::INFO_(), argv[2], __FILE__, __LINE__, __FUNCTION__);
        wrt->write(evt);
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
