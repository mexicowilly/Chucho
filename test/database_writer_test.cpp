/*
 * Copyright 2013-2020 Will Mason
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
#include <chucho/database_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/function_name.hpp>
#include <chucho/logger.hpp>
#include <chucho/environment.hpp>

TEST(database_writer, one_and_marker)
{
    auto env = chucho::environment::get("DB_CONNECTION");
    if (env)
    {
        auto fmt = std::make_unique<chucho::pattern_formatter>("%m");
        chucho::database_writer wrt("database", std::move(fmt), *env);
        chucho::event e(chucho::logger::get("will"), chucho::level::INFO_(), "Hello, World! (monkies)", __FILE__, __LINE__,
                        CHUCHO_FUNCTION_NAME);
        wrt.write(e);
        chucho::event em(chucho::logger::get("will"), chucho::level::INFO_(), "Hello, World! (monkies)", __FILE__, __LINE__,
                        CHUCHO_FUNCTION_NAME, "marky");
        wrt.write(em);
    }
}

