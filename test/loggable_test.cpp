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
#include <chucho/loggable.hpp>
#include <chucho/log.hpp>
#include <iostream>

namespace one
{

namespace two
{

class my_loggable : public chucho::loggable<my_loggable>
{
public:
    my_loggable()
    {
    }

    my_loggable(const std::string& lname)
        : chucho::loggable<my_loggable>(lname)
    {
    }

    std::string get_logger_name() const
    {
        return get_logger()->get_name();
    }
};

class my_other_loggable : public my_loggable
{
public:
    my_other_loggable()
    {
        rename_logger(typeid(my_other_loggable));
    }

    my_other_loggable(const std::string& lname)
    {
        rename_logger(lname);
    }
};

template <int num>
class templated : public chucho::loggable<templated<num>>
{
public:
    templated()
    {
        std::cout << "Logger name: " << this->get_logger()->get_name() << std::endl;
    }

    int get_val() const {
        CHUCHO_INFO_L("Getting " << val_;);
        return val_;
    }

private:
    int val_{num};
};


}

}

TEST(loggable, assigned_name)
{
    one::two::my_loggable my("hello kitty");
    EXPECT_EQ(std::string("hello kitty"), my.get_logger_name());
}

TEST(loggable, name_of_type)
{
    one::two::my_loggable my;
    EXPECT_EQ(std::string("one.two.my_loggable"), my.get_logger_name());
}

TEST(loggable, rename_by_name)
{
    one::two::my_other_loggable oth("huh");
    EXPECT_EQ(std::string("huh"), oth.get_logger_name());
}

TEST(loggable, rename_by_type)
{
    one::two::my_other_loggable oth;
    EXPECT_EQ(std::string("one.two.my_other_loggable"), oth.get_logger_name());
}

TEST(loggable, templated)
{
    one::two::templated<5> t;
    t.get_val();
}
