#include <chucho/event.hpp>
#include <chucho/marker.hpp>

namespace chucho
{

event::event(std::shared_ptr<logger> lgr,
             std::shared_ptr<level> lvl,
             const std::string& msg,
             const char* const file_name,
             unsigned line_number,
             const char* const function_name,
             const optional<marker>& mark)
    : logger_(lgr),
      level_(lvl),
      message_(msg),
      time_(clock_type::now()),
      file_name_(file_name),
      line_number_(line_number),
      function_name_(function_name),
      marker_(mark)
{
}

event::event(std::shared_ptr<logger> lgr,
             std::shared_ptr<level> lvl,
             const std::string& msg,
             const char* const file_name,
             unsigned line_number,
             const char* const function_name,
             const std::string& mark)
    : event(lgr, lvl, msg, file_name, line_number, function_name, marker(mark))
{
}

}
