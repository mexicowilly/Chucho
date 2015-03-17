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

#if !defined(CHUCHO_LOG_STREAM_HPP__)
#define CHUCHO_LOG_STREAM_HPP__

#include <chucho/log_streambuf.hpp>
#include <chucho/level.hpp>
#include <chucho/function_name.hpp>
#include <ostream>

namespace chucho
{
    
class log_stream_base
{
protected:
    log_stream_base(std::shared_ptr<logger> lgr);
    
    log_streambuf buf_;
};
    
class log_stream : public log_stream_base, public std::ostream
{
public:
    log_stream(std::shared_ptr<logger> lgr);
};

#define CHUCHO_M(stream) \
    static_cast<::chucho::log_streambuf*>((stream).rdbuf())->set_location(__FILE__, __LINE__, CHUCHO_FUNCTION_NAME); \
    (stream)
    
std::ostream& trace(std::ostream& ls);
std::ostream& debug(std::ostream& ls);
std::ostream& info(std::ostream& ls);
std::ostream& warn(std::ostream& ls);
std::ostream& error(std::ostream& ls);
std::ostream& fatal(std::ostream& ls);
std::ostream& endm(std::ostream& ls);
    
struct set_level_type
{
    set_level_type(std::shared_ptr<level> lvl) : level_(lvl) { }
    std::shared_ptr<level> level_;
};
    
inline set_level_type set_level(std::shared_ptr<level> lvl)
{
    return set_level_type(lvl);
}
    
std::ostream& operator<< (std::ostream& stream, set_level_type sl);

struct set_marker_type
{
    set_marker_type(const marker& mrk) : marker_(mrk) { }
    marker marker_;
};
    
inline set_marker_type set_marker(const marker& mrk)
{
    return set_marker_type(mrk);
}
    
std::ostream& operator<< (std::ostream& stream, set_marker_type sm);
    
}

#endif