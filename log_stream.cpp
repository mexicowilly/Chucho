/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/log_stream.hpp>

namespace chucho
{
    
log_stream_base::log_stream_base(std::shared_ptr<logger> lgr)
    : buf_(lgr)
{
}
    
log_stream::log_stream(std::shared_ptr<logger> lgr)
    : log_stream_base(lgr),
      std::ostream(&buf_)
{
}

std::ostream& trace(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::TRACE_());
    return ls;
}
    
std::ostream& debug(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::DEBUG_());
    return ls;
}
    
std::ostream& info(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::INFO_());
    return ls;
}
    
std::ostream& warn(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::WARN_());
    return ls;
}
    
std::ostream& error(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::ERROR_());
    return ls;
}
    
std::ostream& fatal(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(level::FATAL_());
    return ls;
}

std::ostream& endm(std::ostream& ls)
{
    auto lsb = dynamic_cast<log_streambuf*>(ls.rdbuf());
    if (lsb != nullptr)
        lsb->flush_message();
    return ls;
}

std::ostream& operator<< (std::ostream& stream, set_level_type sl)
{
    auto lsb = dynamic_cast<log_streambuf*>(stream.rdbuf());
    if (lsb != nullptr)
        lsb->set_level(sl.level_);
    return stream;
}
    
std::ostream& operator<< (std::ostream& stream, set_marker_type sm)
{
    auto lsb = dynamic_cast<log_streambuf*>(stream.rdbuf());
    if (lsb != nullptr)
        lsb->set_marker(sm.marker_);
    return stream;
}
    
}