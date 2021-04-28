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
#include <chucho/file.hpp>
#include <fstream>

TEST(file_test, base_name)
{
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three/").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three///").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/////three///").c_str());
    EXPECT_STREQ(".", chucho::file::base_name("").c_str());
#if defined(CHUCHO_POSIX)
    EXPECT_STREQ("/", chucho::file::base_name("/").c_str());
    EXPECT_STREQ("/", chucho::file::base_name("/////").c_str());
#elif defined(CHUCHO_WINDOWS)
    EXPECT_STREQ("three", chucho::file::base_name("C:\\one\\two\\three").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("C:\\one\\two\\three\\").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("C:\\one\\two\\three\\\\\\").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("C:\\one\\two\\\\\\\\\\three\\\\\\").c_str());
    EXPECT_STREQ("C:\\", chucho::file::base_name("C:\\").c_str());
    EXPECT_STREQ("C:\\", chucho::file::base_name("C:\\\\\\\\\\").c_str());
#endif
}

TEST(file_test, directory_iterator)
{
    std::string dir_name("directory_iterator_test");
    ASSERT_NO_THROW(chucho::file::remove_all(dir_name));
    ASSERT_NO_THROW(chucho::file::create_directory(dir_name));
    std::ofstream s1(dir_name + chucho::file::dir_sep + "one");
    ASSERT_TRUE(s1.is_open());
    s1 << "hi";
    s1.close();
    std::ofstream s2(dir_name + chucho::file::dir_sep + "two");
    ASSERT_TRUE(s2.is_open());
    s2 << "hi";
    s2.close();
    std::ofstream s3(dir_name + chucho::file::dir_sep + "three");
    ASSERT_TRUE(s3.is_open());
    s3 << "hi";
    s3.close();
    chucho::file::directory_iterator itor(dir_name);
    chucho::file::directory_iterator end;
    std::set<std::string> names;
    while (itor != end)
    {
        names.insert(chucho::file::base_name(*itor));
        ++itor;
    }
    EXPECT_EQ(3, names.size());
    EXPECT_EQ(1, names.count("one"));
    EXPECT_EQ(1, names.count("two"));
    EXPECT_EQ(1, names.count("three"));
    ASSERT_NO_THROW(chucho::file::remove_all(dir_name));
}

TEST(file_test, directory_name)
{
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three/").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three////").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two//////three////").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two//////three").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one/two").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one////two///////").c_str());
    EXPECT_STREQ(".", chucho::file::directory_name("").c_str());
    EXPECT_STREQ(".", chucho::file::directory_name("one").c_str());
#if defined(CHUCHO_POSIX)
    EXPECT_STREQ("/", chucho::file::directory_name("/one").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/one/").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/one////").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("//////").c_str());
#elif defined(CHUCHO_WINDOWS)
    EXPECT_STREQ("C:\\one\\two", chucho::file::directory_name("C:\\one\\two\\three").c_str());
    EXPECT_STREQ("C:\\one\\two", chucho::file::directory_name("C:\\one\\two\\three\\").c_str());
    EXPECT_STREQ("C:\\one\\two", chucho::file::directory_name("C:\\one\\two\\three\\\\\\\\").c_str());
    EXPECT_STREQ("C:\\one\\two", chucho::file::directory_name("C:\\one\\two\\\\\\\\\\\\three\\\\\\\\").c_str());
    EXPECT_STREQ("C:\\one\\two", chucho::file::directory_name("C:\\one\\two\\\\\\\\\\\\three").c_str());
    EXPECT_STREQ("C:\\", chucho::file::directory_name("C:\\one").c_str());
    EXPECT_STREQ("C:\\", chucho::file::directory_name("C:\\one\\").c_str());
    EXPECT_STREQ("C:\\", chucho::file::directory_name("C:\\one\\\\\\\\").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one\\two").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one\\\\\\\\two\\\\\\\\\\\\\\").c_str());
    EXPECT_STREQ(".", chucho::file::directory_name("").c_str());
    EXPECT_STREQ("\\", chucho::file::directory_name("\\").c_str());
    EXPECT_STREQ("\\", chucho::file::directory_name("\\\\\\\\\\\\").c_str());
#endif
}

TEST(file_test, home)
{
    auto h = chucho::file::home_directory();
    std::cout << "HOME: " << h << std::endl;
}

TEST(file_test, is_fully_qualified)
{
#if defined(CHUCHO_POSIX)
    EXPECT_TRUE(chucho::file::is_fully_qualified("/one/two"));
    EXPECT_TRUE(chucho::file::is_fully_qualified("/."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("one/two"));
#elif defined(CHUCHO_WINDOWS)
    EXPECT_TRUE(chucho::file::is_fully_qualified("C:\\one\\two"));
    EXPECT_TRUE(chucho::file::is_fully_qualified("C:\\."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("one/two"));
#endif
}

TEST(file_test, multiple_directories)
{
    ASSERT_NO_THROW(chucho::file::remove_all("one"));
    ASSERT_NO_THROW(chucho::file::create_directories("one/two/three"));
    EXPECT_TRUE(chucho::file::exists("one"));
    EXPECT_TRUE(chucho::file::exists("one/two"));
    EXPECT_TRUE(chucho::file::exists("one/two/three"));
    ASSERT_NO_THROW(chucho::file::remove_all("one"));
    EXPECT_FALSE(chucho::file::exists("one"));
    EXPECT_FALSE(chucho::file::exists("one/two"));
    EXPECT_FALSE(chucho::file::exists("one/two/three"));
}

TEST(file_test, non_directory)
{
    std::ofstream f("file_test_non_directory.txt");
    ASSERT_TRUE(f.is_open());
    f << "hello";
    f.close();
    ASSERT_NO_THROW(chucho::file::remove_all("file_test_non_directory.txt"));
    EXPECT_FALSE(chucho::file::exists("file_test_non_directory.txt"));
    f.open("file_test_non_directory.txt");
    ASSERT_TRUE(f.is_open());
    f << "hello";
    f.close();
    ASSERT_NO_THROW(chucho::file::remove("file_test_non_directory.txt"));
    EXPECT_FALSE(chucho::file::exists("file_test_non_directory.txt"));
}

TEST(file_test, single_directory)
{
    ASSERT_NO_THROW(chucho::file::remove("file_test"));
    ASSERT_NO_THROW(chucho::file::create_directory("file_test"));
    EXPECT_TRUE(chucho::file::exists("file_test"));
    ASSERT_NO_THROW(chucho::file::remove("file_test"));
    EXPECT_FALSE(chucho::file::exists("file_test"));
}

TEST(file_test, size)
{
    std::ofstream f("file_test_size.txt");
    ASSERT_TRUE(f.is_open());
    f << "hello";
    f.close();
    EXPECT_EQ(5, chucho::file::size("file_test_size.txt"));
    ASSERT_NO_THROW(chucho::file::remove("file_test_size.txt"));
}

TEST(file_test, temp)
{
    auto t = chucho::file::temporary_directory();
    std::cout << "TEMP: " << t << std::endl;
}

