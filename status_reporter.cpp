/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/status_reporter.hpp>
#include <chucho/status_manager.hpp>

namespace chucho
{

status_reporter::~status_reporter()
{
}

void status_reporter::report_error(const std::string& message, std::exception_ptr ex) const
{
    report_status(status(status::level::ERROR_, message, origin_, ex));
}

void status_reporter::report_info(const std::string& message, std::exception_ptr ex) const
{
    report_status(status(status::level::INFO_, message, origin_, ex));
}

void status_reporter::report_status(const status& st) const
{
    status_manager::get()->add(st);
}

void status_reporter::report_warning(const std::string& message, std::exception_ptr ex) const
{
    report_status(status(status::level::WARNING_, message, origin_, ex));
}

}
