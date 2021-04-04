/*
 * Copyright 2013-2021 Will Mason
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
#include <algorithm>

namespace chucho
{

writer::writer(const std::string& name, std::unique_ptr<formatter>&& fmt)
    : formatter_(std::move(fmt)),
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
    std::lock_guard<std::mutex> lg(guard_);
    filters_.push_back(std::move(flt));
}

void writer::clear_filters()
{
    std::lock_guard<std::mutex> lg(guard_);
    filters_.clear();
}

void writer::flush()
{
}

filter& writer::get_filter(const std::string& name)
{
    std::lock_guard<std::mutex> lg(guard_);
    auto found = std::find_if(filters_.begin(),
                              filters_.end(),
                              [&name] (const std::unique_ptr<filter>& f) { return f->get_name() == name; });
    if (found == filters_.end())
        throw std::invalid_argument("The filter " + name + " could not be found");
    return **found;
}

std::vector<std::string> writer::get_filter_names()
{
    std::vector<std::string> result;
    std::lock_guard<std::mutex> lg(guard_);
    for (const auto& f : filters_)
        result.push_back(f->get_name());
    return result;
}

// Already locked
bool writer::permits(const event& evt)
{
    for (const auto& f : filters_)
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
    std::lock_guard<std::mutex> lg(guard_);
    filters_.remove_if([&name] (const std::unique_ptr<filter>& f) { return f->get_name() == name; });
}

void writer::write(const event& evt)
{
    std::lock_guard<std::mutex> lg(guard_);
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
