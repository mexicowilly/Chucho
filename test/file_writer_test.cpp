#include <gtest/gtest.h>
#include <chucho/file_writer.hpp>
#include <chucho/file.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/status_manager.hpp>
#include <fstream>

class file_writer_test : public ::testing::Test
{
public:
    file_writer_test()
        : file_name_("file_writer_test")
    {
        chucho::status_manager::get()->clear();
    }

    ~file_writer_test()
    {
        std::remove(file_name_.c_str());
    }

    std::shared_ptr<chucho::file_writer> get_writer(chucho::file_writer::on_start start = chucho::file_writer::on_start::APPEND)
    {
        std::shared_ptr<chucho::formatter> f(new chucho::pattern_formatter("%m%n"));
        std::shared_ptr<chucho::file_writer> w = std::make_shared<chucho::file_writer>(f, file_name_, start);
        return w;
    }

protected:
    std::string file_name_;
};

TEST_F(file_writer_test, error)
{
    chucho::file::create_directory(file_name_);
    get_writer();
    EXPECT_EQ(1, chucho::status_manager::get()->get_count());
    chucho::file::remove(file_name_);
}

TEST_F(file_writer_test, open)
{
    auto w = get_writer();
    EXPECT_EQ(0, chucho::status_manager::get()->get_count());
    EXPECT_EQ(file_name_, w->get_file_name());
    EXPECT_EQ(file_name_, w->get_initial_file_name());
}

TEST_F(file_writer_test, truncate)
{
    std::ofstream stream(file_name_.c_str());
    stream << "hello";
    stream.close();
    auto w = get_writer();
    ASSERT_EQ(0, chucho::status_manager::get()->get_count());
    w.reset();
    EXPECT_EQ(5, chucho::file::size(file_name_));
    w = get_writer(chucho::file_writer::on_start::TRUNCATE);
    ASSERT_EQ(0, chucho::status_manager::get()->get_count());
    w.reset();
    EXPECT_EQ(0, chucho::file::size(file_name_));
}

TEST_F(file_writer_test, write)
{
    auto w = get_writer();
    EXPECT_EQ(0, chucho::status_manager::get()->get_count());
    std::shared_ptr<chucho::logger> log = chucho::logger::get("file_writer_test");
    chucho::event evt(log, chucho::INFO_LEVEL, "hello", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    evt = chucho::event(log, chucho::INFO_LEVEL, "goodbye", __FILE__, __LINE__, __FUNCTION__);
    w->write(evt);
    w.reset();
    std::ifstream stream(file_name_.c_str());
    std::string line;
    std::getline(stream, line);
    EXPECT_STREQ("hello", line.c_str());
    std::getline(stream, line);
    EXPECT_STREQ("goodbye", line.c_str());
    std::getline(stream, line);
    EXPECT_TRUE(stream.eof());
}
