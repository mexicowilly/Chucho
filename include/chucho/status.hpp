#if !defined(CHUCHO_STATUS_HPP__)
#define CHUCHO_STATUS_HPP__

#include <chucho/export.hpp>
#include <memory>
#include <chrono>
#include <exception>
#include <ostream>

namespace chucho
{

class CHUCHO_EXPORT status
{
public:
    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_type;

    enum class level
    {
        INFO,
        WARNING,
        ERROR
    };

    status(level lvl,
           const std::string& message,
           const std::string& origin,
           std::exception_ptr ex = std::exception_ptr());

    std::exception_ptr get_exception() const;
    level get_level() const;
    const std::string& get_message() const;
    const std::string& get_origin() const;
    const time_type& get_time() const;

protected:
    friend CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, const status& st);

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

#endif
