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

#include <chucho/writer.hpp>
#include <chucho/exception.hpp>
#include <atomic>
#include <stdexcept>

namespace
{

std::atomic<bool> write_same_thread_guard(false);

}

namespace chucho
{

writer::writer(std::shared_ptr<formatter> fmt)
    : formatter_(fmt)
{
    if (!formatter_)
        throw std::invalid_argument("The formatter cannot be a nullptr");
}

void writer::add_filter(std::shared_ptr<filter> flt)
{
    std::lock_guard<std::mutex> lg(guard_);
    filters_.push_back(flt);
}

void writer::clear_filters()
{
    std::lock_guard<std::mutex> lg(guard_);
    filters_.clear();
}

std::vector<std::shared_ptr<filter>> writer::get_filters()
{
    std::lock_guard<std::mutex> lg(guard_);
    return filters_;
}

bool writer::permits(const event& evt)
{
    for (std::shared_ptr<filter> f : filters_)
    {
        filter::result res = f->evaluate(evt);
        if (res == filter::result::DENY)
            return false;
        if (res == filter::result::ACCEPT)
            return true;
    }
    return true;
}

void writer::write(const event& evt)
{
    if (!write_same_thread_guard)
    {
        write_same_thread_guard.store(true);
        struct sentry
        {
            ~sentry() { write_same_thread_guard.store(false); }
        } s;
        std::lock_guard<std::mutex> lg(guard_);
        try
        {
            if (formatter_ && permits(evt))
                write_impl(evt);
        }
        catch (std::exception& e)
        {
            report_error("Error writing event: " + exception::nested_whats(e));
        }
    }
}

}
