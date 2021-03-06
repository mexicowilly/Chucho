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

#if !defined(CHUCHO_STATUS_HPP_)
#define CHUCHO_STATUS_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/export.h>
#include <memory>
#include <chrono>
#include <exception>
#include <ostream>

namespace chucho
{

/** 
 * @class status status.hpp chucho/status.hpp 
 *
 * Chucho reports what is happening internally using status 
 * messages. These can be monitored by querying the 
 * status_manager or by installing a custom status_observer. 
 *  
 * @ingroup status 
 */
class CHUCHO_EXPORT status
{
public:
    /**
     * The type of clock used for time representation.
     */
    typedef std::chrono::system_clock clock_type;
    /**
     * The type of time point.
     */
    typedef std::chrono::time_point<clock_type> time_type;

    /**
     * The level of this status event. These definitions have a
     * trailing underscore because they can conflict with
     * preprocessor macros defined by Windows.
     */
    enum class level
    {
        /** 
         * Information only. Nothing is wrong. 
         */ 
        INFO_,
        /** 
         * Something went wrong, but not badly enough to inhibit
         * functionality. 
         */
        WARNING_,
        /** 
         * Chucho is prevented from doing something that it should be 
         * doing.
         */
        ERROR_
    };

    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a status event.
     * 
     * @param lvl the level
     * @param message the status message
     * @param origin the status origin, usually the name of the 
     *               class that is responsible for this status
     * @param ex any associated exception
     */
    status(level lvl,
           const std::string& message,
           const std::string& origin,
           std::exception_ptr ex = std::exception_ptr());
    //@}

    /**
     * Return the exception associated with this status.
     * 
     * @return the exception
     */
    std::exception_ptr get_exception() const;
    /**
     * Return the level of this status.
     * 
     * @return the level
     */
    level get_level() const;
    /**
     * Return the message of this status.
     * 
     * @return the message
     */
    const std::string& get_message() const;
    /**
     * Return the origin of this status. The origin is usually the 
     * class name of the object that created the status. 
     * 
     * @return the origin
     */
    const std::string& get_origin() const;
    /**
     * Return the time of this status. The time is the time at which 
     * the status was created. 
     * 
     * @return the time
     */
    const time_type& get_time() const;

protected:
    /**
     * @name Operator
     */
    //@{
    /**
     * Send a status to an output stream.
     *  
     * @param stream the output stream 
     * @param st the status object 
     */
    friend CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, const status& st);
    //@}

private:
    time_type time_;
    level level_;
    std::string message_;
    std::exception_ptr exception_;
    std::string origin_;
};

inline std::exception_ptr status::get_exception() const
{
    return exception_;
}

inline status::level status::get_level() const
{
    return level_;
}

inline const std::string& status::get_message() const
{
    return message_;
}

inline const std::string& status::get_origin() const
{
    return origin_;
}

inline const status::time_type& status::get_time() const
{
    return time_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
