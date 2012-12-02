#include <gtest/gtest.h>
#include <chucho/file.hpp>
#include <fstream>

TEST(file_test, base_name)
{
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three/").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/three///").c_str());
    EXPECT_STREQ("three", chucho::file::base_name("/one/two/////three///").c_str());
    EXPECT_STREQ("/", chucho::file::base_name("/").c_str());
    EXPECT_STREQ("/", chucho::file::base_name("/////").c_str());
    EXPECT_STREQ(".", chucho::file::base_name("").c_str());
}

TEST(file_test, directory_name)
{
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three/").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two/three////").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two//////three////").c_str());
    EXPECT_STREQ("/one/two", chucho::file::directory_name("/one/two//////three").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/one").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/one/").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/one////").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one/two").c_str());
    EXPECT_STREQ("one", chucho::file::directory_name("one////two///////").c_str());
    EXPECT_STREQ(".", chucho::file::directory_name("").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("/").c_str());
    EXPECT_STREQ("/", chucho::file::directory_name("//////").c_str());
    EXPECT_STREQ(".", chucho::file::directory_name("one").c_str());
}

TEST(file_test, is_fully_qualified)
{
    EXPECT_TRUE(chucho::file::is_fully_qualified("/one/two"));
    EXPECT_TRUE(chucho::file::is_fully_qualified("/."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("."));
    EXPECT_FALSE(chucho::file::is_fully_qualified("one/two"));
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
