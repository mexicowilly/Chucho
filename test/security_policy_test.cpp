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

#include <gtest/gtest.h>
#include <chucho/security_policy.hpp>

TEST(security_policy, default_text_max)
{
    chucho::security_policy pol;
    pol.set_default_text_max(4);
    EXPECT_EQ(4, pol.get_default_text_max());
    std::string val;
    EXPECT_NO_THROW(val = pol.validate("what?", "hell"));
    EXPECT_EQ(std::string("hell"), val);
    EXPECT_THROW(pol.validate("what?", "hello"), chucho::exception);
}

TEST(security_policy, override_char_ptr)
{
    chucho::security_policy pol;
    pol.set_text("char_ptr", 4);
    pol.set_text("char_ptr", 700);
    EXPECT_THROW(pol.validate("char_ptr", "hello"), chucho::exception);
    pol.override_text("char_ptr", 5);
    std::string val;
    EXPECT_NO_THROW(val = pol.validate("char_ptr", "hello"));
    EXPECT_EQ(std::string("hello"), val);
}

TEST(security_policy, override_integer)
{
    chucho::security_policy pol;
    pol.set_integer("int", -1, 1);
    pol.set_integer("int", 699, 701);
    EXPECT_THROW(pol.validate("int", 700), chucho::exception);
    pol.override_integer("int", 699, 701);
    int val;
    EXPECT_NO_THROW(val = pol.validate("int", 700));
    EXPECT_EQ(700, val);
}

TEST(security_policy, override_string)
{
    chucho::security_policy pol;
    pol.set_text("string", 4);
    pol.set_text("string", 700);
    EXPECT_THROW(pol.validate("string", std::string("hello")), chucho::exception);
    pol.override_text("string", 5);
    std::string val;
    EXPECT_NO_THROW(val = pol.validate("string", std::string("hello")));
    EXPECT_EQ(std::string("hello"), val);
}

TEST(security_policy, set_char_ptr)
{
    chucho::security_policy pol;
    pol.set_text("char_ptr", 4);
    std::string val;
    EXPECT_NO_THROW(val = pol.validate("char_ptr", std::string("hell")));
    EXPECT_EQ(std::string("hell"), val);
    EXPECT_THROW(pol.validate("char_ptr", std::string("hello")), chucho::exception);
}

TEST(security_policy, set_integer)
{
    chucho::security_policy pol;
    pol.set_integer("int", -1, 1);
    int val;
    EXPECT_NO_THROW(val = pol.validate("int", 0));
    EXPECT_EQ(0, val);
    EXPECT_THROW(pol.validate("int", 700), chucho::exception);
}

TEST(security_policy, set_string)
{
    chucho::security_policy pol;
    pol.set_text("string", 4);
    std::string val;
    EXPECT_NO_THROW(val = pol.validate("string", std::string("hell")));
    EXPECT_EQ(std::string("hell"), val);
    EXPECT_THROW(pol.validate("string", std::string("hello")), chucho::exception);
}

TEST(security_policy, set_unsigned)
{
    chucho::security_policy pol;
    pol.set_integer("unsigned", 1U, 3U);
    unsigned val;
    EXPECT_NO_THROW(val = pol.validate("unsigned", 2));
    EXPECT_EQ(2, val);
    EXPECT_THROW(pol.validate("unsigned", 700), chucho::exception);
}
