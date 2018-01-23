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
#include <chucho/log.hpp>
#include <chucho/pattern_formatter.hpp>
#include <vector>
#include <thread>
#include <sstream>
#include <iostream>

namespace
{

class sink_writer : public chucho::writer
{
public:
    sink_writer();

    void verify(const std::vector<std::string>& expected);
    void verify(std::size_t size);

protected:
    virtual void write_impl(const chucho::event& evt) override;

private:
    std::vector<std::string> messages_;
};

sink_writer::sink_writer()
    : chucho::writer("sink", std::move(std::make_unique<chucho::pattern_formatter>("%m%n")))
{
}

void sink_writer::verify(const std::vector<std::string>& expected)
{
    ASSERT_EQ(expected.size(), messages_.size());
    for (std::size_t i = 0; i < expected.size(); i++)
        EXPECT_EQ(expected[i], messages_[i]);
}

void sink_writer::verify(std::size_t size)
{
    EXPECT_EQ(size, messages_.size());
}

void sink_writer::write_impl(const chucho::event& evt)
{
    messages_.push_back(evt.get_message());
}

void thread_main()
{
    std::ostringstream stream;
    stream << "multithread." << std::this_thread::get_id();
    auto lg = chucho::logger::get(stream.str());
    auto wrt = std::make_unique<sink_writer>();
    lg->add_writer(std::move(wrt));
    std::vector<std::string> expected;
    for (std::size_t i = 0; i < 10000; i++)
    {
        expected.push_back(std::to_string(i));
        CHUCHO_INFO(lg, i);
    }
    dynamic_cast<sink_writer&>(lg->get_writer("sink")).verify(expected);
}

}

TEST(multithread, logs)
{
    chucho::logger::remove_unused_loggers();
    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < 5; i++)
        threads.emplace(threads.end(), thread_main);
    for (std::thread& t : threads)
        t.join();
}
