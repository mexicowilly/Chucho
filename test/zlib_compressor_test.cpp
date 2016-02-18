/*
 * Copyright 2013-2016 Will Mason
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
#include <chucho/zlib_compressor.hpp>

TEST(zlib_compressor, big)
{
    chucho::zlib_compressor cmp;
    std::vector<std::uint8_t> in;
    for (int i = 0; i < 1000000; i++)
        in.push_back('a' + (i % 26));
    std::vector<std::uint8_t> out;
    cmp.compress(in, out);
    cmp.finish(out);
    z_stream z;
    std::memset(&z, 0, sizeof(z));
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    int rc = inflateInit(&z);
    ASSERT_EQ(Z_OK, rc);
    z.next_in = &out[0];
    z.avail_in = out.size();
    std::vector<std::uint8_t> un(in.size());
    z.next_out = &un[0];
    z.avail_out = un.size();
    rc = inflate(&z, Z_FINISH);
    ASSERT_EQ(Z_STREAM_END, rc);
    ASSERT_EQ(0, z.avail_out);
    inflateEnd(&z);
    ASSERT_EQ(in.size(), un.size());
    EXPECT_TRUE(std::equal(in.begin(), in.end(), un.begin()));
}

TEST(zlib_compressor, simple)
{
    chucho::zlib_compressor cmp;
    // VS2012 does not support initializer lists
    std::vector<std::uint8_t> in;
    in.push_back(1);
    in.push_back(2);
    in.push_back(3);
    std::vector<std::uint8_t> out;
    cmp.compress(in, out);
    in.clear();
    in.push_back(4);
    in.push_back(5);
    in.push_back(6);
    cmp.compress(in, out);
    cmp.finish(out);
    ASSERT_GT(out.size(), 0);
    // Can't use the convenience function uncompress() here because when
    // zlib is packaged into the Chucho library, the convenience functions
    // are not included
    std::uint8_t un[1024];
    z_stream z;
    std::memset(&z, 0, sizeof(z));
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    int rc = inflateInit(&z);
    ASSERT_EQ(Z_OK, rc);
    z.next_in = &out[0];
    z.avail_in = out.size();
    z.next_out = un;
    z.avail_out = sizeof(un);
    rc = inflate(&z, Z_FINISH);
    ASSERT_EQ(Z_STREAM_END, rc);
    std::size_t written = sizeof(un) - z.avail_out;
    inflateEnd(&z);
    ASSERT_EQ(6, written);
    EXPECT_EQ(1, un[0]);
    EXPECT_EQ(2, un[1]);
    EXPECT_EQ(3, un[2]);
    EXPECT_EQ(4, un[3]);
    EXPECT_EQ(5, un[4]);
    EXPECT_EQ(6, un[5]);
}