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

#if !defined(CHUCHO_EVENT_HPP__)
#define CHUCHO_EVENT_HPP__

#include <chucho/level.hpp>
#include <chucho/optional.hpp>
#include <chucho/marker.hpp>
#include <string>
#include <chrono>
#include <memory>

namespace chucho
{

class logger;
class marker;

/**
 * @class event event.hpp chucho/event.hpp
 * A log event. You don't need to worry about this. It is used 
 * internally, but required by classes that have a public 
 * interface. 
 *  
 * @ingroup miscellaneous 
 */
class CHUCHO_EXPORT event
{
public:
    /**
     * The type of clock used by event.
     */
    typedef std::chrono::system_clock clock_type;
    /**
     * The type of time_point used by event.
     */
    typedef std::chrono::time_point<clock_type> time_type;

    /**
     * Construct an event.
     * 
     * @param lgr the logger
     * @param lvl the level
     * @param msg the message, which has already been formatted
     * @param file_name the file name of in which the event occurred 
     * @param line_number the line number in which the event 
     *                    occurred
     * @param function_name the function name in which the event 
     *                      occured
     * @param mark the marker associated with the event
     */
    event(std::shared_ptr<logger> lgr,
          std::shared_ptr<level> lvl,
          const std::string& msg,
          const char* const file_name,
          unsigned line_number,
          const char* const function_name,
          const optional<marker>& mark = optional<marker>());
    /**
     * Construct an event.
     * 
     * @param lgr the logger
     * @param lvl the level
     * @param msg the message, which has already been formatted
     * @param file_name the file name of in which the event occurred 
     * @param line_number the line number in which the event 
     *                    occurred
     * @param function_name the function name in which the event 
     *                      occured
     * @param mark the marker associated with the event
     */
    event(std::shared_ptr<logger> lgr,
          std::shared_ptr<level> lvl,
          const std::string& msg,
          const char* const file_name,
          unsigned line_number,
          const char* const function_name,
          const std::string& mark);

    /**
     * Return the file name.
     * 
     * @return the file name
     */
    const char* get_file_name() const;
    /**
     * Return the function name.
     * 
     * @return the function name
     */
    const char* get_function_name() const;
    /**
     * Return the level.
     * 
     * @return the level
     */
    std::shared_ptr<level> get_level() const;
    /**
     * Return the line number.
     * 
     * @return the line number
     */
    unsigned get_line_number() const;
    /**
     * Return the logger.
     * 
     * @return the logger
     */
    std::shared_ptr<logger> get_logger() const;
    /**
     * Return the marker.
     * 
     * @return the marker
     */
    const optional<marker>& get_marker() const;
    /**
     * Return the message.
     * 
     * @return the message
     */
    const std::string& get_message() const;
    /**
     * Return the time that the event was created.
     * 
     * @return the time
     */
    const time_type& get_time() const;

private:
    std::shared_ptr<logger> logger_;
    std::shared_ptr<level> level_;
    std::string message_;
    time_type time_;
    const char* file_name_;
    unsigned line_number_;
    const char* function_name_;
    optional<marker> marker_;
};

inline const char* event::get_file_name() const
{
    return file_name_;
}

inline const char* event::get_function_name() const
{
    return function_name_;
}

inline std::shared_ptr<level> event::get_level() const
{
    return level_;
}

inline unsigned event::get_line_number() const
{
    return line_number_;
}

inline std::shared_ptr<logger> event::get_logger() const
{
    return logger_;
}

inline const optional<marker>& event::get_marker() const
{
    return marker_;
}

inline const std::string& event::get_message() const
{
    return message_;
}

inline const event::time_type& event::get_time() const
{
    return time_;
}

}

#endif
