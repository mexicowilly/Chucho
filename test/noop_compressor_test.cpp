/*
 * Copyright 2013-2018 Will Mason
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
#include <chucho/noop_compressor.hpp>

TEST(noop_compressor, simple)
{
    chucho::noop_compressor cmp;
    // VS2012 does not support initializer lists
    std::vector<std::uint8_t> in;
    in.push_back(1);
    in.push_back(2);
    in.push_back(3);
    auto out = cmp.compress(in);
    ASSERT_EQ(3, out.size());
    EXPECT_EQ(1, out[0]);
    EXPECT_EQ(2, out[1]);
    EXPECT_EQ(3, out[2]);
    in[0] = 4;
    in[1] = 5;
    in[2] = 6;
    out = cmp.compress(in);
    ASSERT_EQ(3, out.size());
    EXPECT_EQ(4, out[0]);
    EXPECT_EQ(5, out[1]);
    EXPECT_EQ(6, out[2]);
}
