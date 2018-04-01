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
#include <chucho/lz4_compressor.hpp>
#include <cstring>
#include <lz4.h>

TEST(lz4_compressor, big)
{
    chucho::lz4_compressor cmp;
    std::vector<std::uint8_t> in;
    for (int i = 0; i < 1000000; i++)
        in.push_back('a' + (i % 26));
    auto out = cmp.compress(in);
    ASSERT_GT(out.size(), 0);
    std::vector<std::uint8_t> un(in.size());
    auto rc = LZ4_decompress_safe(reinterpret_cast<const char*>(&out[0]),
                                  reinterpret_cast<char*>(&un[0]),
                                  out.size(),
                                  un.size());
    ASSERT_EQ(un.size(), rc);
    EXPECT_TRUE(in == un);
}

TEST(lz4_compressor, simple)
{
    chucho::lz4_compressor cmp;
    // VS2012 does not support initializer lists
    std::vector<std::uint8_t> in;
    in.push_back(1);
    in.push_back(2);
    in.push_back(3);
    auto out = cmp.compress(in);
    ASSERT_GT(out.size(), 0);
    std::vector<std::uint8_t> un(in.size());
    auto rc = LZ4_decompress_safe(reinterpret_cast<const char*>(&out[0]),
                                  reinterpret_cast<char*>(&un[0]),
                                  out.size(),
                                  un.size());
    ASSERT_EQ(3, rc);
    EXPECT_EQ(1, un[0]);
    EXPECT_EQ(2, un[1]);
    EXPECT_EQ(3, un[2]);
}

