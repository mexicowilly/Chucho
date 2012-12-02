#include <chucho/status.hpp>

namespace chucho
{

status::status(level lvl,
               const std::string& message,
               std::exception_ptr ex)
    : time_(clock_type::now()),
      level_(lvl),
      message_(message),
      exception_(ex)
{
}

std::ostream& operator<< (std::ostream& stream, const status& st)
{
    if (st.level_ == status::level::INFO)
        stream << "INFO";
    else if (st.level_ == status::level::WARN)
        stream << "WARN";
    else if (st.level_ == status::level::ERROR)
        stream << "ERROR";
    stream << ' ' << st.message_;
    if (st.exception_ != std::exception_ptr())
    {
        try
        {
            std::rethrow_exception(st.exception_);
        }
        catch (std::exception& e)
        {
            stream << e.what();
        }
    }
    return stream;
}

}
