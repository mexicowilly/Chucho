#include <chucho/level.hpp>
#include <chucho/exception.hpp>
#include <limits>
#include <vector>

namespace chucho
{

std::shared_ptr<level> TRACE_LEVEL(new trace_level());
std::shared_ptr<level> DEBUG_LEVEL(new debug_level());
std::shared_ptr<level> INFO_LEVEL(new info_level());
std::shared_ptr<level> WARN_LEVEL(new warn_level());
std::shared_ptr<level> ERROR_LEVEL(new error_level());
std::shared_ptr<level> FATAL_LEVEL(new fatal_level());
std::shared_ptr<level> OFF_LEVEL(new off_level());

std::ostream& operator<< (std::ostream& stream, const level& lvl)
{
    stream << lvl.get_name();
    return stream;
}

level::~level()
{
}

std::shared_ptr<level> level::from_text(const std::string& text)
{
    std::string up;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(up),
                   (int(*)(int))std::toupper);
    std::vector<std::shared_ptr<level>> lvls =
    {
        TRACE_LEVEL,
        DEBUG_LEVEL,
        INFO_LEVEL,
        WARN_LEVEL,
        ERROR_LEVEL,
        FATAL_LEVEL,
        OFF_LEVEL
    };
    for (std::shared_ptr<level> lvl : lvls)
    {
        if (up == lvl->get_name())
            return lvl;
    }
    throw exception("The text " + text + " does not describe a valid log level");
}

const char* trace_level::get_name() const
{
    return "TRACE";
}

int trace_level::get_value() const
{
    return 0;
}

const char* debug_level::get_name() const
{
    return "DEBUG";
}

int debug_level::get_value() const
{
    return 10000;
}

const char* info_level::get_name() const
{
    return "INFO";
}

int info_level::get_value() const
{
    return 20000;
}

const char* warn_level::get_name() const
{
    return "WARN";
}

int warn_level::get_value() const
{
    return 30000;
}

const char* error_level::get_name() const
{
    return "ERROR";
}

int error_level::get_value() const
{
    return 40000;
}

const char* fatal_level::get_name() const
{
    return "FATAL";
}

int fatal_level::get_value() const
{
    return 50000;
}

const char* off_level::get_name() const
{
    return "OFF";
}

int off_level::get_value() const
{
    return std::numeric_limits<int>::max();
}

}
