#include <chucho/event.hpp>

namespace chucho
{

event::event(std::shared_ptr<logger> lgr,
             std::shared_ptr<level> lvl,
             const std::string& msg,
             const char* const file_name,
             unsigned line_number,
             const char* const function_name)
    : logger_(lgr),
      level_(lvl),
      message_(msg),
      time_(clock_type::now()),
      file_name_(file_name),
      line_number_(line_number),
      function_name_(function_name)
{
}

}
