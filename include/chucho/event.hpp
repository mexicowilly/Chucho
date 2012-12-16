#if !defined(CHUCHO_EVENT_HPP__)
#define CHUCHO_EVENT_HPP__

#include <chucho/level.hpp>
#include <string>
#include <chrono>
#include <memory>

namespace chucho
{

class logger;

class CHUCHO_EXPORT event
{
public:
    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_type;

    event(std::shared_ptr<logger> lgr,
          std::shared_ptr<level> lvl,
          const std::string& msg,
          const char* const file_name,
          unsigned line_number,
          const char* const function_name);

    const char* get_file_name() const;
    const char* get_function_name() const;
    std::shared_ptr<level> get_level() const;
    unsigned get_line_number() const;
    std::shared_ptr<logger> get_logger() const;
    const std::string& get_message() const;
    const time_type& get_time() const;

private:
    std::shared_ptr<logger> logger_;
    std::shared_ptr<level> level_;
    std::string message_;
    time_type time_;
    const char* file_name_;
    unsigned line_number_;
    const char* function_name_;
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

inline const std::string& event::get_message() const
{
    return message_;
}

inline const event::time_type& event::get_time() const
{
    return time_;
}

}

#endif
