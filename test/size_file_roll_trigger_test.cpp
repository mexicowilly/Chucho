#include <gtest/gtest.h>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/logger.hpp>
#include <fstream>

TEST(size_file_roll_trigger_test, trigger)
{
    chucho::size_file_roll_trigger t(512);
    chucho::event evt(chucho::logger::get_logger("size_file_roll_trigger_test"),
                      chucho::INFO_LEVEL,
                      "hello",
                      __FILE__,
                      __LINE__,
                      __FUNCTION__);
    std::string name("size_file_roll_trigger_test");
    EXPECT_FALSE(t.is_triggered(name, evt));
    std::vector<char> v(1024, 'W');
    std::ofstream stream(name);
    stream.write(&v[0], 1);
    stream.flush();
    EXPECT_FALSE(t.is_triggered(name, evt));
    stream.write(&v[0], 510);
    stream.flush();
    EXPECT_FALSE(t.is_triggered(name, evt));
    stream.write(&v[0], 1);
    stream.flush();
    EXPECT_TRUE(t.is_triggered(name, evt));
    stream.write(&v[0], 510);
    stream.flush();
    EXPECT_TRUE(t.is_triggered(name, evt));
    stream.close();
    std::remove(name.c_str());
}
