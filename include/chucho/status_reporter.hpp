/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_STATUS_REPORTER_HPP__)
#define CHUCHO_STATUS_REPORTER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/status.hpp>

namespace chucho
{

/**
 * @class status_reporter status_reporter.hpp chucho/status_reporter.hpp
 * Report chucho's internal status. Status reporters use the 
 * @ref status_manager to report events that occur during the 
 * course of chucho's operation. For example, any errors in the 
 * configuration file will appear in chuch's status history. 
 *  
 * @ingroup status 
 */
class CHUCHO_EXPORT status_reporter
{
public:
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy a status_reporter.
     */
    virtual ~status_reporter();
    //@}

protected:
    /**
     * Return the origin of these status messages. The origin is 
     * usually the name of the class that inherits from this class. 
     * 
     * @return the origin of messages
     */
    const std::string& get_status_origin() const;
    /**
     * Report an error.
     * 
     * @param message the error message
     * @param ex a possible exception associated with this error
     */
    void report_error(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    /**
     * Report information.
     * 
     * @param message the message
     * @param ex a possible exception associated with this status
     */
    void report_info(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    /**
     * Report a status that you made yourself.
     * 
     * @param st the status
     */
    void report_status(const status& st) const;
    /**
     * Report a warning.
     * 
     * @param message the warning message
     * @param ex a possible exception associated with this warning
     */
    void report_warning(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    /**
     * Set the origin of status messages. The origin is usually the 
     * name of the class that inherits from this class. 
     * 
     * @param origin the origin of status messages
     */
    void set_status_origin(const std::string& origin);

private:
    std::string origin_;
};

inline const std::string& status_reporter::get_status_origin() const
{
    return origin_;
}

inline void status_reporter::set_status_origin(const std::string& origin)
{
    origin_ = origin;
} 

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
