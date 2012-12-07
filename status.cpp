#include <chucho/status.hpp>
#include <chucho/calendar.hpp>
#include <array>

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
    auto since = st.time_.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since);
    std::array<char, 13> time_fmt;
    std::snprintf(time_fmt.data(), time_fmt.size(), "%%H:%%M:%%S.%03lli", millis.count() % 1000);
    struct std::tm cal = calendar::get_utc(millis.count() / 1000);
    std::array<char, 1024> buf;
    std::strftime(buf.data(), buf.size(), time_fmt.data(), &cal);
    stream << buf.data() << ' ';
    if (st.level_ == status::level::INFO)
        stream << "INFO";
    else if (st.level_ == status::level::WARNING)
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
            stream << std::endl << "  exception: " << e.what();
        }
    }
    return stream;
}

}
