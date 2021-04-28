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
#include <chucho/environment.hpp>
#include <chucho/loggly_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/function_name.hpp>

TEST(loggly_writer, many)
{
    auto toke = chucho::environment::get("LOGGLY_TOKEN");
    if (!toke)
    {
        std::cout << "Set the LOGGLY_TOKEN envrionment variable to run Loggly tests" << std::endl;
        return;
    }
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    chucho::loggly_writer wrt("loggly", std::move(fmt), *toke);
    for (int i = 0; i < 10000; i++)
    {
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), std::to_string(i), __FILE__, __LINE__,
                        CHUCHO_FUNCTION_NAME);
        wrt.write(e);
    }
}

TEST(loggly_writer, one)
{
    auto toke = chucho::environment::get("LOGGLY_TOKEN");
    if (!toke)
    {
        std::cout << "Set the LOGGLY_TOKEN envrionment variable to run Loggly tests" << std::endl;
        return;
    }
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
    chucho::loggly_writer wrt("loggly", std::move(fmt), *toke);
    wrt.set_verbose(true);
    chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), "Hello, World!", __FILE__, __LINE__, CHUCHO_FUNCTION_NAME);
    wrt.write(e);
}
