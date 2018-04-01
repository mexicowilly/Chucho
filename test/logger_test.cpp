/*
 * Copyright 2013-2018 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <gtest/gtest.h>
#include <chucho/logger.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/cerr_writer.hpp>
#include <algorithm>

class log_test : public ::testing::Test
{
public:
    log_test()
    {
        chucho::logger::remove_unused_loggers();
    }
};

namespace one
{

namespace two
{

namespace three
{

class loggable_type
{
};

}

}

}

TEST_F(log_test, existing_loggers)
{
    chucho::logger::remove_unused_loggers();
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
    root->set_level(chucho::level::WARN_());
    EXPECT_EQ(chucho::level::WARN_(), root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
    EXPECT_EQ(chucho::level::WARN_(), root->get_level());
    std::shared_ptr<chucho::logger> one = chucho::logger::get("one");
    EXPECT_EQ(chucho::level::WARN_(), one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    root->set_level(chucho::level::INFO_());
    EXPECT_EQ(chucho::level::INFO_(), one->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(one->get_level()));
    std::shared_ptr<chucho::logger> two = chucho::logger::get("one.two");
    EXPECT_EQ(chucho::level::INFO_(), two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    one->set_level(chucho::level::WARN_());
    ASSERT_TRUE(static_cast<bool>(one->get_level()));
    EXPECT_EQ(chucho::level::WARN_(), one->get_level());
    EXPECT_EQ(chucho::level::WARN_(), two->get_effective_level());
    ASSERT_FALSE(static_cast<bool>(two->get_level()));
    EXPECT_EQ(chucho::level::INFO_(), root->get_effective_level());
    ASSERT_TRUE(static_cast<bool>(root->get_level()));
}

TEST_F(log_test, remove_writers)
{
    auto lgr = chucho::logger::get("remove_writers");
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m%n");
    auto out = std::make_unique<chucho::cout_writer>("out", std::move(fmt));
    fmt = std::make_unique<chucho::pattern_formatter>("%m%n");
    auto err = std::make_unique<chucho::cerr_writer>("err", std::move(fmt));
    lgr->add_writer(std::move(out));
    lgr->add_writer(std::move(err));
    auto wrts = lgr->get_writer_names();
    EXPECT_EQ(2, wrts.size());
    lgr->remove_writer("out");
    wrts = lgr->get_writer_names();
    EXPECT_EQ(1, wrts.size());
    EXPECT_STREQ(typeid(chucho::cerr_writer).name(), typeid(lgr->get_writer(wrts[0])).name());
    fmt = std::make_unique<chucho::pattern_formatter>("%m%n");
    auto out2 = std::make_unique<chucho::cout_writer>("out2", std::move(fmt));
    lgr->add_writer(std::move(out2));
    wrts = lgr->get_writer_names();
    EXPECT_EQ(2, wrts.size());
    lgr->clear_writers();
    EXPECT_TRUE(lgr->get_writer_names().empty());
}

TEST_F(log_test, reset)
{
    auto lgr = chucho::logger::get("reset");
    auto fmt = std::make_unique<chucho::pattern_formatter>("%m%n");
    lgr->add_writer(std::move(std::make_unique<chucho::cout_writer>("out", std::move(fmt))));
    lgr->set_level(chucho::level::WARN_());
    lgr->set_writes_to_ancestors(false);
    EXPECT_EQ(1, lgr->get_writer_names().size());
    EXPECT_EQ(chucho::level::WARN_(), lgr->get_level());
    EXPECT_FALSE(lgr->writes_to_ancestors());
    lgr->reset();
    EXPECT_TRUE(lgr->get_writer_names().empty());
    EXPECT_FALSE(static_cast<bool>(lgr->get_level()));
    EXPECT_TRUE(lgr->writes_to_ancestors());
}

TEST_F(log_test, type_to_logger_name)
{
    std::string name = chucho::logger::type_to_logger_name(typeid(one::two::three::loggable_type));
    EXPECT_EQ(std::string("one.two.three.loggable_type"), name);
}
