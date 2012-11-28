#include <gtest/gtest.h>
#include <chucho/logger.hpp>

TEST(log_test, existing_loggers)
{
    std::shared_ptr<chucho::logger> l = chucho::logger::get_logger("eight.nine.ten.eleven");
    std::vector<std::shared_ptr<chucho::logger> > all = chucho::logger::get_existing_loggers();
    EXPECT_EQ(all.size(), 5);
    EXPECT_FALSE(std::find_if(all.begin(), all.end(),
                              [](std::shared_ptr<chucho::logger> l) { return l->get_name() ==  ""; }) == all.end());
    EXPECT_FALSE(std::find_if(all.begin(), all.end(),
                              [](std::shared_ptr<chucho::logger> l) { return l->get_name() ==  "eight"; }) == all.end());
    EXPECT_FALSE(std::find_if(all.begin(), all.end(),
                              [](std::shared_ptr<chucho::logger> l) { return l->get_name() ==  "eight.nine"; }) == all.end());
    EXPECT_FALSE(std::find_if(all.begin(), all.end(),
                              [](std::shared_ptr<chucho::logger> l) { return l->get_name() ==  "eight.nine.ten"; }) == all.end());
    EXPECT_FALSE(std::find_if(all.begin(), all.end(),
                              [](std::shared_ptr<chucho::logger> l) { return l->get_name() ==  "eight.nine.ten.eleven"; }) == all.end());
}

TEST(log_test, levels)
{
    std::shared_ptr<chucho::logger> root = chucho::logger::get_logger("");
    std::shared_ptr<chucho::level> warn(std::make_shared<chucho::warn_level>());
    root->set_level(warn);
    EXPECT_EQ(warn, root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
    EXPECT_EQ(warn, root->get_level());
    std::shared_ptr<chucho::logger> one = chucho::logger::get_logger("one");
    EXPECT_EQ(warn, one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    std::shared_ptr<chucho::level> info(std::make_shared<chucho::info_level>());
    root->set_level(info);
    EXPECT_EQ(info, one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    std::shared_ptr<chucho::logger> two = chucho::logger::get_logger("one.two");
    EXPECT_EQ(info, two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    one->set_level(warn);
    ASSERT_TRUE(static_cast<bool>(one->get_level()));
    EXPECT_EQ(warn, one->get_level());
    EXPECT_EQ(warn, two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    EXPECT_EQ(info, root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
}

TEST(log_test, sizeof_weak_ptr)
{
    std::cout << "sizeof std::weak_ptr<chucho::logger> " << sizeof(std::weak_ptr<chucho::logger>) << std::endl;
}


