/*
 * Copyright 2013 Will Mason
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
#include <chucho/log.hpp>
#include <chucho/pattern_formatter.hpp>
#include <vector>
#include <thread>
#include <sstream>
#include <iostream>

namespace
{

std::condition_variable logger_created_condition;
std::mutex logger_created_mutex;

class sink_writer : public chucho::writer
{
public:
    sink_writer();

    void verify(const std::vector<std::string>& expected);

protected:
    virtual void write_impl(const chucho::event& evt) override;

private:
    std::vector<std::string> messages_;
};

sink_writer::sink_writer()
    : chucho::writer(std::make_shared<chucho::pattern_formatter>("%m%n"))
{
}

void sink_writer::verify(const std::vector<std::string>& expected)
{
    ASSERT_EQ(expected.size(), messages_.size());
    for (std::size_t i = 0; i < expected.size(); i++)
        EXPECT_EQ(expected[i], messages_[i]);
}

void sink_writer::write_impl(const chucho::event& evt)
{
    messages_.push_back(evt.get_message());
}

void thread_main()
{
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    auto lg = chucho::logger::get(stream.str());
    logger_created_condition.notify_one();
    auto wrt = std::make_shared<sink_writer>();
    lg->add_writer(wrt);
    std::vector<std::string> expected;
    for (std::size_t i = 0; i < 10000; i++)
    {
        expected.push_back(std::to_string(i));
        CHUCHO_INFO(lg, i);
    }
    wrt->verify(expected);
}

}

TEST(multithread, logs)
{
    std::vector<std::thread> threads;
    std::unique_lock<std::mutex> ul(logger_created_mutex);
    for (std::size_t i = 0; i < 5; i++) {
        threads.emplace(threads.end(), thread_main);
        logger_created_condition.wait(ul);
    }
    ul.unlock();
    for (std::thread& t : threads)
        t.join();
}
