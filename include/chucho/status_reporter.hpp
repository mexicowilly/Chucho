#if !defined(CHUCHO_STATUS_REPORTER_HPP__)
#define CHUCHO_STATUS_REPORTER_HPP__

#include <chucho/status.hpp>

namespace chucho
{

class CHUCHO_EXPORT status_reporter
{
public:
    virtual ~status_reporter();

protected:
    const std::string& get_status_origin() const;
    void report_error(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    void report_info(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    void report_status(const status& st) const;
    void report_warning(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
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
