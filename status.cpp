#include <chucho/status.hpp>
#include <chucho/calendar.hpp>
#include <chucho/exception.hpp>
#include <chucho/clock_util.hpp>
#include <array>

#include <iostream>

namespace chucho
{

status::status(level lvl,
               const std::string& message,
               const std::string& origin,
               std::exception_ptr ex)
    : time_(clock_type::now()),
      level_(lvl),
      message_(message),
      exception_(ex),
      origin_(origin)
{
}

std::ostream& operator<< (std::ostream& stream, const status& st)
{
    std::array<char, 1024> buf;
    if (clock_util::system_clock_supports_milliseconds)
    {
        auto since = st.time_.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since);
        std::array<char, 13> time_fmt;
        std::snprintf(time_fmt.data(), time_fmt.size(), "%%H:%%M:%%S.%03lli", millis.count() % 1000);
        struct std::tm cal = calendar::get_utc(millis.count() / 1000);
        std::strftime(buf.data(), buf.size(), time_fmt.data(), &cal);
    }
    else
    {
        struct std::tm cal = calendar::get_utc(status::clock_type::to_time_t(st.time_));
        std::strftime(buf.data(), buf.size(), "%H:%M:%S", &cal);

    }
    stream << buf.data() << ' ';
    if (st.level_ == status::level::INFO)
        stream << "INFO";
    else if (st.level_ == status::level::WARNING)
        stream << "WARN";
    else if (st.level_ == status::level::ERROR)
        stream << "ERROR";
    if (!st.origin_.empty())
        stream << " [" << st.origin_ << ']';
    stream << ' ' << st.message_;
    if (st.exception_ != std::exception_ptr())
    {
        stream << std::endl << "  exception: ";
        try
        {
            std::rethrow_exception(st.exception_);
        }
        catch (exception& e)
        {
            stream << exception::nested_whats(e);
        }
        catch (std::exception& se)
        {
            stream << se.what();
        }
    }
    return stream;
}

}
