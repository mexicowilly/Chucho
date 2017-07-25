/*
 * Copyright 2013-2017 Will Mason
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
#include <chucho/async_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chrono>
#include <vector>

namespace
{

class slow_writer : public chucho::writer
{
public:
    slow_writer(std::chrono::milliseconds delay);

    const std::vector<std::string>& get_events() const;

protected:
    virtual void write_impl(const chucho::event& evt) override;

private:
    std::vector<std::string> events_;
    std::chrono::milliseconds delay_;
};

class async_writer_test : public ::testing::Test
{
public:
    chucho::event get_event(const std::string msg, std::shared_ptr<chucho::level> lvl = chucho::level::INFO_());
    std::shared_ptr<chucho::async_writer> get_writer(std::chrono::milliseconds millis,
                                                     std::size_t capacity = chucho::async_writer::DEFAULT_QUEUE_CAPACITY);
};

slow_writer::slow_writer(std::chrono::milliseconds delay)
    : chucho::writer(std::make_shared<chucho::pattern_formatter>("%m")),
      delay_(delay)
{
}

const std::vector<std::string>& slow_writer::get_events() const
{
    return events_;
}

void slow_writer::write_impl(const chucho::event& evt)
{
    std::this_thread::sleep_for(delay_);
    events_.push_back(formatter_->format(evt));
}

chucho::event async_writer_test::get_event(const std::string msg, std::shared_ptr<chucho::level> lvl)
{
    return chucho::event(chucho::logger::get("will"),
                         lvl,
                         msg,
                         __FILE__,
                         __LINE__,
                         __FUNCTION__);
}

std::shared_ptr<chucho::async_writer> async_writer_test::get_writer(std::chrono::milliseconds millis,
                                                                    std::size_t capacity)
{
    auto wrt = std::make_shared<slow_writer>(millis);
    auto as = std::make_shared<chucho::async_writer>(wrt, capacity);
    return as;
}

}

TEST_F(async_writer_test, blocking)
{
    auto as = get_writer(std::chrono::milliseconds(50), 5);
    EXPECT_EQ(5, as->get_queue_capacity());
    EXPECT_EQ(chucho::level::INFO_(), as->get_discard_threshold());
    for (int i = 0; i < 20; i++)
        as->write(get_event(std::to_string(i), chucho::level::WARN_()));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    auto slow = std::dynamic_pointer_cast<slow_writer>(as->get_writer());
    ASSERT_EQ(20, slow->get_events().size());
    for (int i = 0; i < 20; i++)
        EXPECT_EQ(i, std::stoi(slow->get_events()[i]));
}

TEST_F(async_writer_test, discard)
{
    auto as = get_writer(std::chrono::milliseconds(50), 5);
    EXPECT_EQ(5, as->get_queue_capacity());
    EXPECT_EQ(chucho::level::INFO_(), as->get_discard_threshold());
    for (int i = 0; i < 20; i++)
    {
        std::shared_ptr<chucho::level> lvl = chucho::level::INFO_();
        if (i & 1)
            lvl = chucho::level::WARN_();
        as->write(get_event(std::to_string(i), lvl));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto slow = std::dynamic_pointer_cast<slow_writer>(as->get_writer());
    EXPECT_LT(slow->get_events().size(), 20U);
    std::cout << "Got " << slow->get_events().size() << " { ";
    for (auto evt : slow->get_events())
        std::cout << evt << ' ';
    std::cout << '}' << std::endl;
}

TEST_F(async_writer_test, slow)
{
    auto as = get_writer(std::chrono::milliseconds(50));
    EXPECT_EQ(chucho::async_writer::DEFAULT_QUEUE_CAPACITY, as->get_queue_capacity());
    EXPECT_EQ(chucho::level::INFO_(), as->get_discard_threshold());
    for (int i = 0; i < 10; i++)
        as->write(get_event(std::to_string(i)));
    // we don't know how many are there
    EXPECT_GT(as->get_queue_size(), 0U);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(0, as->get_queue_size());
    auto slow = std::dynamic_pointer_cast<slow_writer>(as->get_writer());
    ASSERT_EQ(10, slow->get_events().size());
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(i, std::stoi(slow->get_events()[i]));
}
