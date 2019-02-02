/*
 * Copyright 2013-2019 Will Mason
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
#include <chucho/lzma_compressor.hpp>
#include <cstring>
#include <lzma.h>

TEST(lzma_compressor, big)
{
    chucho::lzma_compressor cmp;
    std::vector<std::uint8_t> in;
    for (int i = 0; i < 1000000; i++)
        in.push_back('a' + (i % 26));
    auto out = cmp.compress(in);
    ASSERT_GT(out.size(), 0);
    std::vector<std::uint8_t> un(in.size());
    std::uint64_t mem_limit = 10 * 1024 * 1024;
    std::size_t in_pos = 0;
    std::size_t out_pos = 0;
    auto rc = lzma_stream_buffer_decode(&mem_limit,
                                        LZMA_TELL_NO_CHECK | LZMA_TELL_UNSUPPORTED_CHECK,
                                        nullptr,
                                        &out[0],
                                        &in_pos,
                                        out.size(),
                                        &un[0],
                                        &out_pos,
                                        un.size());
    ASSERT_EQ(LZMA_OK, rc);
    ASSERT_EQ(un.size(), out_pos);
    EXPECT_TRUE(in == un);
}

TEST(lzma_compressor, simple)
{
    chucho::lzma_compressor cmp;
    // VS2012 does not support initializer lists
    std::vector<std::uint8_t> in;
    in.push_back(1);
    in.push_back(2);
    in.push_back(3);
    auto out = cmp.compress(in);
    ASSERT_GT(out.size(), 0);
    std::vector<std::uint8_t> un(in.size());
    std::uint64_t mem_limit = 10 * 1024 * 1024;
    std::size_t in_pos = 0;
    std::size_t out_pos = 0;
    auto rc = lzma_stream_buffer_decode(&mem_limit,
                                        LZMA_TELL_NO_CHECK | LZMA_TELL_UNSUPPORTED_CHECK,
                                        nullptr,
                                        &out[0],
                                        &in_pos,
                                        out.size(),
                                        &un[0],
                                        &out_pos,
                                        un.size());
    ASSERT_EQ(LZMA_OK, rc);
    ASSERT_EQ(3, out_pos);
    EXPECT_EQ(1, un[0]);
    EXPECT_EQ(2, un[1]);
    EXPECT_EQ(3, un[2]);
}
