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
