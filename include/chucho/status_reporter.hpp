#if !defined(CHUCHO_STATUS_REPORTER_HPP__)
#define CHUCHO_STATUS_REPORTER_HPP__

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

#endif
