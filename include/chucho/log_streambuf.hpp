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

#if !defined(CHUCHO_LOG_STREAMBUF_HPP__)
#define CHUCHO_LOG_STREAMBUF_HPP__

#include <chucho/logger.hpp>
#include <chucho/marker.hpp>
#include <streambuf>

namespace chucho
{

class log_streambuf : public std::streambuf,
                      public status_reporter
{
public:
    log_streambuf(std::shared_ptr<logger> lgr);

    void flush_message();
    std::shared_ptr<level> get_level() const;
    virtual int_type overflow(int_type ch) override;
    void set_level(std::shared_ptr<level> lvl);
    void set_location(const char* const file_name,
                      unsigned line_number,
                      const char* const function_name);
    void set_marker(const marker& mrk);
    
private:
    std::shared_ptr<logger> logger_;
    std::string message_;
    std::shared_ptr<level> level_;
    optional<marker> marker_;
    const char* file_name_;
    unsigned line_number_;
    const char* function_name_;
};

inline std::shared_ptr<level> log_streambuf::get_level() const
{
    return level_;
}

inline void log_streambuf::set_level(std::shared_ptr<level> lvl)
{
    level_ = lvl;
}

inline void log_streambuf::set_marker(const marker& mrk)
{
    marker_ = mrk;
}

}

#endif