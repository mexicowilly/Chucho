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

#include <chucho/pattern_formatter.hpp>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <unistd.h>

namespace chucho
{

pattern_formatter::base_host_piece::base_host_piece(const format_params& params)
    : piece(params)
{
    struct utsname info;
    uname(&info);
    name_ = info.nodename;
}

std::string pattern_formatter::milliseconds_since_start_piece::get_text_impl(const event& evt) const
{
    std::string result;
    struct rusage info;
    if (getrusage(RUSAGE_SELF, &info) == 0)
        result = std::to_string(info.ru_utime.tv_sec * 1000 + info.ru_utime.tv_usec / 1000);
    return result;
}

std::string pattern_formatter::pid_piece::get_text_impl(const event& evt) const
{
    return std::to_string(getpid());
}

}
