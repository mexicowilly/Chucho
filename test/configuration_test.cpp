#include <gtest/gtest.h>
#include <chucho/configuration.hpp>
#include <chucho/file.hpp>
#include <chucho/logger.hpp>
#include <chucho/level.hpp>
#include <chucho/cout_writer.hpp>
#include <fstream>

namespace
{

class configuration : public ::testing::Test
{
public:
    ~configuration()
    {
        try
        {
            chucho::file::remove(get_file_name());
        }
        catch (std::exception&)
        {
        }
    }

    std::string get_file_name() const
    {
        return "configuration_test.yaml";
    }
};

}

TEST_F(configuration, perform)
{
    /*
    std::ofstream stream(get_file_name().c_str());
    stream << "chucho::logger:\n";
    stream << "    - name: conf\n";
    stream << "    - level: off\n";
    stream << "    - chucho::cout_writer:\n";
    stream << "        - chucho::pattern_formatter:\n";
    stream << "            - pattern: '%m%n'\n";
    stream.close();
    chucho::configuration::set_file_name(get_file_name());
    auto lgr = chucho::logger::get("conf");
    ASSERT_TRUE(static_cast<bool>(lgr->get_level()));
    EXPECT_EQ(*chucho::level::OFF, *lgr->get_level());
    auto wrts = lgr->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cout_writer), typeid(*wrts[0]));
    */
}
