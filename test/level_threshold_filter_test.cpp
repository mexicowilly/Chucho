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
    chucho::level_threshold_filter f(chucho::INFO_LEVEL);
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::TRACE_LEVEL)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::DEBUG_LEVEL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::INFO_LEVEL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::WARN_LEVEL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::ERROR_LEVEL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::FATAL_LEVEL)));
    EXPECT_EQ(chucho::filter::result::NEUTRAL, f.evaluate(get_event(chucho::OFF_LEVEL)));
}
