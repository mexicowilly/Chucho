#include <chucho/status_reporter.hpp>
#include <chucho/status_manager.hpp>

namespace chucho
{

status_reporter::~status_reporter()
{
}

void status_reporter::report_error(const std::string& message, std::exception_ptr ex)
{
    report_status(status(status::level::ERROR, message, origin_, ex));
}

void status_reporter::report_info(const std::string& message, std::exception_ptr ex)
{
    report_status(status(status::level::INFO, message, origin_, ex));
}

void status_reporter::report_status(const status& st)
{
    status_manager::get()->add(st);
}

void status_reporter::report_warning(const std::string& message, std::exception_ptr ex)
{
    report_status(status(status::level::WARNING, message, origin_, ex));
}

}
