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

#include <chucho/writer.hpp>
#include <chucho/exception.hpp>
#include <stdexcept>

namespace chucho
{

writer::writer(const std::string& name, std::unique_ptr<formatter>&& fmt)
    : formatter_(std::move(fmt)),
      guard_(std::make_unique<std::recursive_mutex>()),
      i_am_writing_(false),
      name_(name)
{
    if (!formatter_)
        throw std::invalid_argument("The formatter cannot be a nullptr");
}

writer::~writer()
{
}

void writer::add_filter(std::unique_ptr<filter>&& flt)
{
    std::lock_guard<std::recursive_mutex> lg(*guard_);
    filters_.push_back(std::move(flt));
}

void writer::clear_filters()
{
    std::lock_guard<std::recursive_mutex> lg(*guard_);
    filters_.clear();
}

void writer::flush()
{
}

std::vector<std::string> writer::get_filter_names()
{
    std::vector<std::string> result;
    std::lock_guard<std::recursive_mutex> lg(*guard_);
    for (const auto& f : filters_)
        result.push_back(f->get_name());
    return result;
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

void writer::remove_filter(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lg(*guard_);
    filters_.remove_if([&name] (const std::unique_ptr<filter>& f) { f->get_name() == name; });
}

void writer::write(const event& evt)
{
    std::lock_guard<std::recursive_mutex> lg(*guard_);
    // Prevent writing more than once in same thread
    if (!i_am_writing_)
    {
        i_am_writing_ = true;
        struct sentry
        {
            sentry(bool& i_am_writing) : i_am_writing_(i_am_writing) { }
            ~sentry() { i_am_writing_ = false; }
            bool& i_am_writing_;
        } s(i_am_writing_);
        try
        {
            if (permits(evt))
                write_impl(evt);
        }
        catch (std::exception& e)
        {
            report_error("Error writing event: " + exception::nested_whats(e));
        }
    }
}

}
