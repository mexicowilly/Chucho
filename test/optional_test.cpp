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
#include <chucho/optional.hpp>

TEST(optional_test, basic)
{
    chucho::optional<int> one;
    EXPECT_FALSE(one);
    one = 2;
    EXPECT_TRUE(one);
    EXPECT_EQ(2, *one);
    chucho::optional<int> two(3);
    EXPECT_EQ(3, *two);
    one = two;
    EXPECT_EQ(3, *one);
    one = chucho::optional<int>();
    EXPECT_FALSE(one);
}
