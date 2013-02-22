#include <gtest/gtest.h>
#include <chucho/regex.hpp>

TEST(regex, iterator)
{
    chucho::regex::expression re("\\$([Ee][Nn][Vv])?\\{(.+)\\}");
    chucho::regex::iterator i("I ${HAVE} one and $ENV{IT} looks good.", re);
    chucho::regex::iterator end;
    EXPECT_NE(i, end);
    chucho::regex::iterator j = i;
    EXPECT_EQ(i, j);
    chucho::regex::match m(*i);
    EXPECT_EQ(3, m.size());
    EXPECT_EQ(2, m[0].begin());
    EXPECT_EQ(7, m[0].length());
    EXPECT_LT(m[1].begin(), 0);
    EXPECT_EQ(4, m[2].begin());
    EXPECT_EQ(4, m[2].length());
    chucho::regex::iterator k = i++;
    EXPECT_EQ(j, k);
    m = *i;
    EXPECT_EQ(3, m.size());
    EXPECT_EQ(18, m[0].begin());
    EXPECT_EQ(8, m[0].length());
    EXPECT_EQ(19, m[1].begin());
    EXPECT_EQ(3, m[1].length());
    EXPECT_EQ(23, m[2].begin());
    EXPECT_EQ(2, m[2].length());
    k = ++i;
    EXPECT_EQ(i, k);
    EXPECT_EQ(end, i);
    i = chucho::regex::iterator("chucho::${MY_TYPE_IS}", re);
    EXPECT_NE(end, i);
    m = *i;
    ASSERT_EQ(3, m.size());
    EXPECT_EQ(8, m[0].begin());
    EXPECT_EQ(13, m[0].length());
    EXPECT_LT(m[1].begin(), 0);
    EXPECT_EQ(10, m[2].begin());
    EXPECT_EQ(10, m[2].length());
    chucho::regex::expression re2("fleas");
    i = chucho::regex::iterator("my dog has fleas", re2);
    m = *i;
    EXPECT_EQ(11, m[0].begin());
    EXPECT_EQ(5, m[0].length());
    i = chucho::regex::iterator("my dog has cats", re2);
    EXPECT_EQ(end, i);
}

TEST(regex, replace)
{
    chucho::regex::expression re("w[0-9]m");
    std::string rep = chucho::regex::replace("I am w1m and also w2m", re, "pepe");
    EXPECT_EQ(std::string("I am pepe and also pepe"), rep);
    rep = chucho::regex::replace("hello, w2m, check trailing", re, "doggy");
    EXPECT_EQ(std::string("hello, doggy, check trailing"), rep);
}

TEST(regex, search)
{
    chucho::regex::expression re("d.g");
    EXPECT_TRUE(chucho::regex::search("my dog has fleas", re));
}
