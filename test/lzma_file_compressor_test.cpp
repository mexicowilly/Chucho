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
#include <chucho/lzma_file_compressor.hpp>
#include <chucho/file.hpp>
#include <fstream>
#include <algorithm>

namespace
{

void make_file(const std::string& name)
{
    std::ofstream stream(name.c_str());
    for (int i = 0; i < (100 * 1024) / (26 * 2); i++)
    {
        std::string chars("abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ");
        std::random_shuffle(chars.begin(), chars.end());
        stream << chars;
    }
}

}

TEST(lzma_file_compressor, compress)
{
    std::string name("lzma_file_compressor_test");
    make_file(name);
    chucho::lzma_file_compressor xz(12);
    EXPECT_EQ(12, xz.get_min_index());
    EXPECT_STREQ(".xz", xz.get_extension());
    ASSERT_TRUE(chucho::file::exists(name));
    ASSERT_NO_THROW(xz.compress(name));
    EXPECT_FALSE(chucho::file::exists(name));
    name += ".xz";
    EXPECT_TRUE(chucho::file::exists(name));
    chucho::file::remove(name);
}

