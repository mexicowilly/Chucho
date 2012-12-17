#include <gtest/gtest.h>
#include <chucho/marker.hpp>
#include <sstream>

TEST(marker_test, children)
{
    chucho::marker mark("hello");
    auto one = std::make_shared<chucho::marker>("one");
    mark.insert(one);
    auto two = std::make_shared<chucho::marker>("two");
    mark.insert(two);
    EXPECT_FALSE(mark.empty());
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), one) == mark.end());
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), two) == mark.end());
    auto three = std::make_shared<chucho::marker>("three");
    EXPECT_TRUE(std::find(mark.begin(), mark.end(), three) == mark.end());
    auto two2 = std::make_shared<chucho::marker>("two");
    EXPECT_FALSE(std::find(mark.begin(), mark.end(), two) == mark.end());
}

TEST(marker_test, format)
{
    chucho::marker mark("one");
    mark.insert(std::make_shared<chucho::marker>("1"));
    mark.insert(std::make_shared<chucho::marker>("2"));
    std::ostringstream stream;
    stream << mark;
    EXPECT_STREQ("one [ 1, 2 ]", stream.str().c_str());
}

TEST(marker_test, name)
{
    chucho::marker mark("one");
    EXPECT_STREQ("one", mark.get_name().c_str());
}

TEST(marker_test, sort)
{
    chucho::marker mark1("one");
    chucho::marker mark2("two");
    EXPECT_LT(mark1, mark2);
    EXPECT_FALSE(mark1 == mark2);
}
