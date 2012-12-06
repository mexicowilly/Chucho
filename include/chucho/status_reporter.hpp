#if !defined(CHUCHO_STATUS_REPORTER_HPP__)
#define CHUCHO_STATUS_REPORTER_HPP__

#include <chucho/status_manager.hpp>

namespace chucho
{

class CHUCHO_EXPORT status_reporter
{
public:
    virtual ~status_reporter();

protected:
    void report_error(const std::string& message, std::exception_ptr ex = std::exception_ptr());
    void report_info(const std::string& message, std::exception_ptr ex = std::exception_ptr());
    void report_status(const status& st);
    void report_warning(const std::string& message, std::exception_ptr ex = std::exception_ptr());

private:
    std::shared_ptr<status_manager> smgr_;
};

}

#endif
