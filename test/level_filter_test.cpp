#include <gtest/gtest.h>
#include <chucho/level_filter.hpp>
#include <chucho/logger.hpp>

namespace
{

class level_filter_test : public ::testing::Test
{
protected:
    level_filter_test()
        : logger_(chucho::logger::get("level_filter_test"))
    {
    }

    chucho::event get_event(std::shared_ptr<chucho::level> lvl)
    {
        return chucho::event(logger_, lvl, "hello", __FILE__, __LINE__, __FUNCTION__);
    }

private:
    std::shared_ptr<chucho::logger> logger_;
};

}

TEST_F(level_filter_test, evaluate)
{
    chucho::level_filter f(chucho::level::ERROR, chucho::filter::result::ACCEPT, chucho::filter::result::DENY);
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::TRACE)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::DEBUG)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::INFO)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::WARN)));
    EXPECT_EQ(chucho::filter::result::ACCEPT, f.evaluate(get_event(chucho::level::ERROR)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::FATAL)));
    EXPECT_EQ(chucho::filter::result::DENY, f.evaluate(get_event(chucho::level::OFF)));
}
