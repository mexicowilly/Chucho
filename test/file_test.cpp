#include <gtest/gtest.h>
#include <chucho/file.hpp>

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
}

TEST(file_test, single_directory)
{
    ASSERT_NO_THROW(chucho::file::remove_directory("file_test"));
    ASSERT_NO_THROW(chucho::file::create_directory("file_test"));
    EXPECT_TRUE(chucho::file::exists("file_test"));
    ASSERT_NO_THROW(chucho::file::remove_directory("file_test"));
}
