#if !defined(CHUCHO_FILE_ROLL_TRIGGER_HPP__)
#define CHUCHO_FILE_ROLL_TRIGGER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>

namespace chucho
{

/**
 * @class file_roll_trigger file_roll_trigger.hpp chucho/file_roll_trigger.hpp 
 * Determine when a file should roll. A file_roll_trigger is 
 * used by a @ref rolling_file_writer to determine when a file 
 * should roll. 
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT file_roll_trigger : public virtual status_reporter,
                                        public virtual configurable
{
public:
    /**
     * Return whether now is a good time to roll a log file.
     *
     * @param active_file the file that is currently being logged by 
     *                    the writer that owns this trigger
     * @param e the log event currently being written
     * @return true if the file should be rolled now
     */
    virtual bool is_triggered(const std::string& active_file, const event& e) = 0;
};

}

#endif
