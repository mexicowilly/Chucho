#include <gtest/gtest.h>
#include <chucho/logger.hpp>

class log_test : public ::testing::Test
{
public:
    log_test()
    {
        chucho::logger::remove_unused_loggers();
    }
};

TEST_F(log_test, existing_loggers)
{
    std::shared_ptr<chucho::logger> l = chucho::logger::get("eight.nine.ten.eleven");
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

TEST_F(log_test, levels)
{
    std::shared_ptr<chucho::logger> root = chucho::logger::get("");
    root->set_level(chucho::level::WARN);
    EXPECT_EQ(chucho::level::WARN, root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
    EXPECT_EQ(chucho::level::WARN, root->get_level());
    std::shared_ptr<chucho::logger> one = chucho::logger::get("one");
    EXPECT_EQ(chucho::level::WARN, one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    root->set_level(chucho::level::INFO);
    EXPECT_EQ(chucho::level::INFO, one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    std::shared_ptr<chucho::logger> two = chucho::logger::get("one.two");
    EXPECT_EQ(chucho::level::INFO, two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    one->set_level(chucho::level::WARN);
    ASSERT_TRUE(static_cast<bool>(one->get_level()));
    EXPECT_EQ(chucho::level::WARN, one->get_level());
    EXPECT_EQ(chucho::level::WARN, two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    EXPECT_EQ(chucho::level::INFO, root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
}
