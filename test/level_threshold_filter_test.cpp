#include <gtest/gtest.h>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/logger.hpp>

class level_threshold_filter_test : public ::testing::Test
{
protected:
    level_threshold_filter_test()
        : logger_(chucho::logger::get("level_threshold_filter_test"))
    {
    }

    chucho::event get_event(std::shared_ptr<chucho::level> lvl)
    {
        return chucho::event(logger_, lvl, "hello", __FILE__, __LINE__, __FUNCTION__);
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

TEST_F(level_threshold_filter_test, evaluate)
{
    chucho::level_threshold_filter f(chucho::level::INFO);
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::TRACE)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::DEBUG)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::INFO)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::WARN)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::ERROR)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::FATAL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::level::OFF)));
}
