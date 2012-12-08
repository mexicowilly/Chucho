#include <gtest/gtest.h>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/numbered_file_roller.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/file.hpp>
#include <chucho/logger.hpp>

class rolling_file_writer_test : public ::testing::Test
{
protected:
    rolling_file_writer_test()
        : logger_(chucho::logger::get_logger("rolling_file_writer_test")),
          dir_name_("rolling_file_writer_test")
    {
        chucho::file::create_directory(dir_name_);
    }

    ~rolling_file_writer_test()
    {
        try
        {
            chucho::file::remove_all(dir_name_);
        }
        catch (...)
        {
        }
    }

    chucho::event get_event(const std::string& msg)
    {
        return chucho::event(logger_, msg, __FILE__, __LINE__, __FUNCTION__);
    }

    std::string get_file_name(const std::string& base)
    {
        return dir_name_ + '/' + base;
    }

    std::string get_line(const std::string& file_name)
    {
        std::ifstream stream(file_name);
        std::string line;
        std::getline(stream, line);
        return line;
    }

private:
    std::shared_ptr<chucho::logger> logger_;
    std::string dir_name_;
};

TEST_F(rolling_file_writer_test, numbered)
{
    std::shared_ptr<chucho::file_roll_trigger> trig(std::make_shared<chucho::size_file_roll_trigger>(5));
    std::unique_ptr<chucho::file_roller> roll(new chucho::numbered_file_roller(1, 2));
    std::string fn = get_file_name("num");
    chucho::rolling_file_writer w(fn, std::move(roll), trig);
    w.set_formatter(std::make_shared<chucho::pattern_formatter>("%m%n"));
    w.write(get_event("one:hello"));
    w.write(get_event("two:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("two:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("one:hello", get_line(fn + ".1").c_str());
    w.write(get_event("three:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("three:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("two:hello", get_line(fn + ".1").c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".2"));
    EXPECT_STREQ("one:hello", get_line(fn + ".2").c_str());
    w.write(get_event("four:hello"));
    EXPECT_TRUE(chucho::file::exists(fn));
    EXPECT_STREQ("four:hello", get_line(fn).c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".1"));
    EXPECT_STREQ("three:hello", get_line(fn + ".1").c_str());
    EXPECT_TRUE(chucho::file::exists(fn + ".2"));
    EXPECT_STREQ("two:hello", get_line(fn + ".2").c_str());
    EXPECT_FALSE(chucho::file::exists(fn + ".3"));
}
