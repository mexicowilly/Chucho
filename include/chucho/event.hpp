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

#if !defined(CHUCHO_EVENT_HPP_)
#define CHUCHO_EVENT_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

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
     * @name Constructors
     */
    //@{
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
     * Construct an event.
     *
     * @param evt the event to copy
     */
    event(const event& evt);
    //@}

    /**
     * Copy an event.
     *
     * @param evt to copy
     * @return this
     */
    event& operator= (const event& evt);

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
     * Return the thread ID. This will only be set if the
     * event has been read from an event cache.
     *
     * @return the ID
     */
    const optional<std::string>& get_thread_id() const;
    /**
     * Return the time that the event was created.
     * 
     * @return the time
     */
    const time_type& get_time() const;

private:
    friend class event_cache;

    std::shared_ptr<logger> logger_;
    std::shared_ptr<level> level_;
    std::string message_;
    time_type time_;
    const char* file_name_;
    unsigned line_number_;
    const char* function_name_;
    optional<marker> marker_;
    // These below are used by the event_cache.
    optional<std::string> thread_id_;
    optional<std::string> file_name_store_;
    optional<std::string> function_name_store_;
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

inline const optional<std::string>& event::get_thread_id() const
{
    return thread_id_;
}

inline const event::time_type& event::get_time() const
{
    return time_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
