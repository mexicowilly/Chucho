#if !defined(CHUCHO_FILE_ROLL_TRIGGER_HPP__)
#define CHUCHO_FILE_ROLL_TRIGGER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>

namespace chucho
{

class CHUCHO_EXPORT file_roll_trigger : public status_reporter
{
public:
    virtual bool is_triggered(const std::string& active_file, const event& e) = 0;
};

}

#endif
