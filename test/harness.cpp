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

#include <gtest/gtest.h>
#include <chucho/status_manager.hpp>
#include <chucho/configuration.hpp>
#include <chucho/finalize.hpp>
#include <iostream>
#include <fstream>

class all_status : public chucho::status_observer
{
public:
    virtual void status_reported(const chucho::status& st) override
    {
        std::cout << st << std::endl;
    }
};

class global_fixture : public ::testing::Environment
{
public:
    virtual void SetUp() override
    {
        chucho::configuration::set_allow_default(false);
        observer_.reset(new all_status());
        chucho::status_manager::get()->add(observer_);
    }

    virtual void TearDown() override
    {
        chucho::finalize();
    }

private:
    std::shared_ptr<chucho::status_observer> observer_;
};

int main(int argc, char* argv[])
{
    if (argc == 3 && strcmp(argv[1], "--location") == 0)
    {
        std::ofstream stream(argv[2]);
        if (!stream.is_open())
            return EXIT_FAILURE;
        stream << argv[0];
        return EXIT_SUCCESS;
    }
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new global_fixture());
    return RUN_ALL_TESTS();
}
