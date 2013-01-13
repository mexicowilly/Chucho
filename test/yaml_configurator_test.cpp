#include <gtest/gtest.h>
#include <chucho/yaml_configurator.hpp>
#include <chucho/logger.hpp>
#include <chucho/status_manager.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/rolling_file_writer.hpp>
#include <chucho/numbered_file_roller.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/time_file_roller.hpp>
#include <sstream>

class yaml_configurator : public ::testing::Test
{
public:
    yaml_configurator()
    {
        chucho::logger::remove_unused_loggers();
        // This ensures that all factories are registered
        chucho::logger::get("");
        chucho::status_manager::get()->clear();
    }

    ~yaml_configurator()
    {
        EXPECT_LT(chucho::status_manager::get()->get_level(), chucho::status::level::WARNING);
    }

protected:
    void configure(const char* const yaml)
    {
        std::istringstream stream(yaml);
        cnf_.configure(stream);
    }

private:
    chucho::yaml_configurator cnf_;
};

TEST_F(yaml_configurator, cerr_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::cerr_writer:\n"
              "        - chucho::pattern_formatter:\n"
              "            - pattern: '%m%n'");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cerr_writer), typeid(*wrts[0]));
}

TEST_F(yaml_configurator, cout_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    EXPECT_EQ(typeid(chucho::cout_writer), typeid(*wrts[0]));
}

TEST_F(yaml_configurator, file_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: hello.log\n"
              "        on_start: truncate\n"
              "        flush: false");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("hello.log"), fwrt->get_file_name());
    EXPECT_FALSE(fwrt->get_flush());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt->get_on_start());
}

TEST_F(yaml_configurator, level_threshold_filter)
{
    configure("- chucho::logger:\n"
              "    name: will\n"
              "    chucho::cout_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::level_threshold_filter:\n"
              "            level: fatal");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    auto flts = wrts[0]->get_filters();
    ASSERT_EQ(1, flts.size());
    ASSERT_EQ(typeid(chucho::level_threshold_filter), typeid(*flts[0]));
    auto thresh = std::static_pointer_cast<chucho::level_threshold_filter>(flts[0]);
    EXPECT_EQ(*chucho::FATAL_LEVEL, *thresh->get_level());
}

TEST_F(yaml_configurator, logger)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    level: fatal\n"
              "    writes_to_ancestors: false");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    EXPECT_EQ(*chucho::FATAL_LEVEL, *lgr->get_level());
    EXPECT_EQ(false, lgr->writes_to_ancestors());
}

TEST_F(yaml_configurator, multiple_writer)
{
    configure("chucho::logger:\n"
              "    - name: will\n"
              "    - chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: one.log\n"
              "    - chucho::file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        file_name: two.log\n");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(2, wrts.size());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("one.log"), fwrt->get_file_name());
    EXPECT_EQ(typeid(chucho::file_writer), typeid(*wrts[1]));
    fwrt = std::static_pointer_cast<chucho::file_writer>(wrts[1]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("two.log"), fwrt->get_file_name());
}

TEST_F(yaml_configurator, rolling_file_writer)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::numbered_file_roller:\n"
              "            min_index: 3\n"
              "            max_index: 5\n"
              "        chucho::size_file_roll_trigger:\n"
              "            max_size: 5000\n"
              "        file_name: what.log\n"
              "        on_start: TruNCAte\n"
              "        flush: false");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    EXPECT_EQ(std::string("what.log"), fwrt->get_file_name());
    EXPECT_EQ(chucho::file_writer::on_start::TRUNCATE, fwrt->get_on_start());
    EXPECT_FALSE(fwrt->get_flush());
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::numbered_file_roller), typeid(*rlr));
    auto nrlr = std::static_pointer_cast<chucho::numbered_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(nrlr));
    EXPECT_EQ(3, nrlr->get_min_index());
    EXPECT_EQ(5, nrlr->get_max_index());
    auto trg = fwrt->get_file_roll_trigger();
    ASSERT_EQ(typeid(chucho::size_file_roll_trigger), typeid(*trg));
    auto strg = std::static_pointer_cast<chucho::size_file_roll_trigger>(trg);
    ASSERT_TRUE(static_cast<bool>(strg));
    EXPECT_EQ(5000, strg->get_max_size());
}

TEST_F(yaml_configurator, time_file_roller)
{
    configure("chucho::logger:\n"
              "    name: will\n"
              "    chucho::rolling_file_writer:\n"
              "        chucho::pattern_formatter:\n"
              "            pattern: '%m%n'\n"
              "        chucho::time_file_roller:\n"
              "            file_name_pattern: '%d{%d}'\n"
              "            max_history: 5\n");
    auto wrts = chucho::logger::get("will")->get_writers();
    ASSERT_EQ(1, wrts.size());
    ASSERT_EQ(typeid(chucho::rolling_file_writer), typeid(*wrts[0]));
    auto fwrt = std::static_pointer_cast<chucho::rolling_file_writer>(wrts[0]);
    ASSERT_TRUE(static_cast<bool>(fwrt));
    auto rlr = fwrt->get_file_roller();
    ASSERT_EQ(typeid(chucho::time_file_roller), typeid(*rlr));
    auto trlr = std::static_pointer_cast<chucho::time_file_roller>(rlr);
    ASSERT_TRUE(static_cast<bool>(trlr));
    EXPECT_EQ(std::string("%d{%d}"), trlr->get_file_name_pattern());
    EXPECT_EQ(5, trlr->get_max_history());
    EXPECT_EQ(trlr, fwrt->get_file_roll_trigger());
}

TEST_F(yaml_configurator, variables)
{
    configure("- variables:\n"
              "    MY_NAME_IS: will\n"
              "    MY_TYPE_IS: logger\n"
              "    MY_KEY_IS: level\n"
              "    MY_VALUE_IS: fatal\n"
              "- 'chucho::${MY_TYPE_IS}':\n"
              "    name: '${MY_NAME_IS}'\n"
              "    '${MY_KEY_IS}': '${MY_VALUE_IS}'\n"
              "    writes_to_ancestors: false");
    std::shared_ptr<chucho::logger> lgr = chucho::logger::get("will");
    EXPECT_EQ(std::string("will"), lgr->get_name());
    EXPECT_EQ(*chucho::FATAL_LEVEL, *lgr->get_level());
    EXPECT_EQ(false, lgr->writes_to_ancestors());
}
