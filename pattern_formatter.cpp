#include <chucho/pattern_formatter.hpp>
#include <limits>
#include <sstream>
#include <array>
#include <mutex>
#include <thread>

namespace
{

enum class parser_state
{
    BEGIN,
    LITERAL,
    DOT,
    MIN,
    MAX
};

std::mutex calendar_guard;

}

namespace chucho
{

pattern_exception::pattern_exception(const std::string& msg)
    : exception(msg)
{
}

pattern_formatter::pattern_formatter(const std::string& pattern)
{
    parse(pattern);
}

std::shared_ptr<pattern_formatter::piece> pattern_formatter::create_piece(std::string::const_iterator& pos,
                                                                          std::string::const_iterator end,
                                                                          justification just,
                                                                          std::size_t min_width,
                                                                          std::size_t max_width)
{
    std::shared_ptr<piece> result;
    std::string arg;
    char c = *pos;
    switch (c)
    {
    case 'b':
        result.reset(new base_file_piece(just, min_width, max_width));
        break;
    case 'c':
        result.reset(new logger_piece(just, min_width, max_width));
        break;
    case 'd':
    case 'D':
        arg = get_argument(pos, end);
        if (arg.empty())
            arg = "%Y-%m-%d %H:%M:%S";
        if (c == 'd')
            result.reset(new utc_date_time_piece(arg, just, min_width, max_width));
        else
            result.reset(new local_date_time_piece(arg, just, min_width, max_width));
        break;
    case 'F':
        result.reset(new full_file_piece(just, min_width, max_width));
        break;
    case 'h':
        result.reset(new base_host_piece(just, min_width, max_width));
        break;
    case 'i':
        result.reset(new pid_piece(just, min_width, max_width));
        break;
    case 'L':
        result.reset(new line_number_piece(just, min_width, max_width));
        break;
    case 'm':
        result.reset(new message_piece(just, min_width, max_width));
        break;
    case 'M':
        result.reset(new function_piece(just, min_width, max_width));
        break;
    case 'n':
        result.reset(new end_of_line_piece(just, min_width, max_width));
        break;
    case 'p':
        result.reset(new level_piece(just, min_width, max_width));
        break;
    case 'r':
        result.reset(new milliseconds_since_start_piece(just, min_width, max_width));
        break;
    case 't':
        result.reset(new thread_piece(just, min_width, max_width));
        break;
    default:
        throw pattern_exception(std::string("Unexpected character '") + c + "' in pattern");
    }
    return result;
}

std::string pattern_formatter::format(const event& evt)
{
    std::string result;
    for (std::shared_ptr<piece>& p : pieces_)
        result += p->get_text(evt);
    return result;
}

std::string pattern_formatter::get_argument(std::string::const_iterator& pos,
                                            std::string::const_iterator end)
{
    std::string result;
    if (pos != end && *(pos + 1) == '{')
    {
        auto last = std::find(++pos, end, '}');
        if (last == end)
        {
            throw pattern_exception("Expected '}' in the pattern");
        }
        else
        {
            result.assign(pos + 1, last);
            pos = last;
        }
    }
    return result;
}

void pattern_formatter::parse(const std::string& pattern)
{
    if (pattern.empty())
        throw pattern_exception("Empty pattern");
    parser_state state = parser_state::LITERAL;
    auto last_char = pattern.end() - 1;
    std::string text;
    justification just = justification::UNSET;
    std::size_t min_width = 0;
    std::size_t max_width = std::numeric_limits<std::size_t>::max();
    for (auto i = pattern.begin(); i != pattern.end(); i++)
    {
        switch (state)
        {
        case parser_state::LITERAL:
            if (i == last_char)
            {
                text += *i;
                continue;
            }
            if (*i == '%')
            {
                if (*(i + 1) == '%')
                {
                    text += *i++;
                }
                else
                {
                    if (!text.empty())
                    {
                        std::shared_ptr<piece> p(new literal_piece(text));
                        pieces_.push_back(p);
                    }
                    state = parser_state::BEGIN;
                }
            }
            else
            {
                text += *i;
            }
            break;
        case parser_state::BEGIN:
            if (*i == '-')
            {
                just = justification::LEFT;
            }
            else
            {
                if (just == justification::UNSET)
                    just = justification::RIGHT;
                if (*i == '.')
                {
                    state = parser_state::DOT;
                }
                else if (*i >= '0' && *i <= '9')
                {
                    text = *i;
                    state = parser_state::MIN;
                }
                else
                {
                    pieces_.push_back(create_piece(i, pattern.end(), just, min_width, max_width));
                    min_width = 0;
                    max_width = std::numeric_limits<std::size_t>::max();
                    state = parser_state::LITERAL;
                    text.clear();
                    just = justification::UNSET;
                }
            }
            break;
        case parser_state::MIN:
            if (*i >= '0' && *i <= '9')
            {
                text = *i;
            }
            else
            {
                min_width = std::stoi(text);
                if (*i == '.')
                {
                    state = parser_state::DOT;
                }
                else
                {
                    pieces_.push_back(create_piece(i, pattern.end(), just, min_width, max_width));
                    min_width = 0;
                    max_width = std::numeric_limits<std::size_t>::max();
                    state = parser_state::LITERAL;
                    text.clear();
                    just = justification::UNSET;
                }
            }
            break;
        case parser_state::DOT:
            if (*i >= '0' && *i <= '9')
            {
                text = *i;
                state = parser_state::MAX;
            }
            else
            {
                std::ostringstream stream;
                stream << "Invalid pattern: Expected '.' but got '" <<
                    *i << "'";
                throw pattern_exception(stream.str());
            }
            break;
        case parser_state::MAX:
            if (*i >= '0' && *i <= '9')
            {
                text += *i;
            }
            else
            {
                max_width = std::stoi(text);
                pieces_.push_back(create_piece(i, pattern.end(), just, min_width, max_width));
                min_width = 0;
                max_width = std::numeric_limits<std::size_t>::max();
                state = parser_state::LITERAL;
                text.clear();
                just = justification::UNSET;
            }
            break;
        }
    }
    if (!text.empty())
    {
        std::shared_ptr<piece> p(new literal_piece(text));
        pieces_.push_back(p);
    }
}

pattern_formatter::piece::piece(justification just,
                                std::size_t min_width,
                                std::size_t max_width)
    : just_(just),
      min_width_(min_width),
      max_width_(max_width)
{
}

std::string pattern_formatter::piece::get_text(const event& evt) const
{
    std::string result = get_text_impl(evt);
    if (result.length() > max_width_)
    {
        result.erase(result.begin(), result.begin() + (result.length() - max_width_));
    }
    else if (result.length() < min_width_)
    {
        std::size_t pad = min_width_ - result.length();
        if (just_ == justification::LEFT)
            result.append(pad, ' ');
        else
            result.insert(0, pad, ' ');
    }
    return result;
}

pattern_formatter::base_file_piece::base_file_piece(justification just,
                                                    std::size_t min_width,
                                                    std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::base_file_piece::get_text_impl(const event& evt) const
{
    #if defined(_WIN32)
    char sep = '\\';
    #else
    char sep = '/';
    #endif
    const char* fn = evt.get_file_name();
    char* found = strrchr(fn, sep);
    return (found == nullptr) ? fn : found + 1;
}

pattern_formatter::full_file_piece::full_file_piece(justification just,
                                                    std::size_t min_width,
                                                    std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::full_file_piece::get_text_impl(const event& evt) const
{
    return evt.get_file_name();
}

pattern_formatter::logger_piece::logger_piece(justification just,
                                              std::size_t min_width,
                                              std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::logger_piece::get_text_impl(const event& evt) const
{
    return evt.get_logger()->get_name();
}

pattern_formatter::date_time_piece::date_time_piece(const std::string& date_pattern,
                                                    justification just,
                                                    std::size_t min_width,
                                                    std::size_t max_width)
    : piece(just, min_width, max_width),
      date_pattern_(date_pattern)
{
    std::size_t pos = date_pattern.find("%q", 0);
    while (pos != std::string::npos)
    {
        if (pos == 0 || date_pattern[pos - 1] != '%')
            milli_positions_.push_back(pos);
        pos = date_pattern.find("%q", pos + 1);
    }
}

std::string pattern_formatter::date_time_piece::get_text_impl(const event& evt) const
{
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(evt.get_time().time_since_epoch());
    std::string pat = date_pattern_;
    if (!milli_positions_.empty())
    {
        std::array<char, 4> buf;
        std::snprintf(buf.data(), buf.size(), "%03lli", millis.count() % 1000);
        for (std::size_t p : milli_positions_)
            pat.replace(p, 2, buf.data());
    }
    std::array<char, 4096> buf;
    struct std::tm cal;
    to_calendar(millis.count() / 1000, cal);
    std::string result;
    if (std::strftime(buf.data(), buf.size(), pat.c_str(), &cal) != 0)
        result = buf.data();
    return result;
}

pattern_formatter::utc_date_time_piece::utc_date_time_piece(const std::string& date_pattern,
                                                            justification just,
                                                            std::size_t min_width,
                                                            std::size_t max_width)
    : date_time_piece(date_pattern, just, min_width, max_width)
{
}

void pattern_formatter::utc_date_time_piece::to_calendar(time_t t, struct std::tm& cal) const
{
    std::lock_guard<std::mutex> lg(calendar_guard);
    cal = *std::gmtime(&t);
}

pattern_formatter::local_date_time_piece::local_date_time_piece(const std::string& date_pattern,
                                                                justification just,
                                                                std::size_t min_width,
                                                                std::size_t max_width)
    : date_time_piece(date_pattern, just, min_width, max_width)
{
}

void pattern_formatter::local_date_time_piece::to_calendar(time_t t, struct std::tm& cal) const
{
    std::lock_guard<std::mutex> lg(calendar_guard);
    cal = *std::localtime(&t);
}

std::string pattern_formatter::base_host_piece::get_text_impl(const event& evt) const
{
    return name_;
}

pattern_formatter::line_number_piece::line_number_piece(justification just,
                                                        std::size_t min_width,
                                                        std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::line_number_piece::get_text_impl(const event& evt) const
{
    return std::to_string(evt.get_line_number());
}

pattern_formatter::message_piece::message_piece(justification just,
                                                std::size_t min_width,
                                                std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::message_piece::get_text_impl(const event& evt) const
{
    return evt.get_message();
}

pattern_formatter::function_piece::function_piece(justification just,
                                                  std::size_t min_width,
                                                  std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::function_piece::get_text_impl(const event& evt) const
{
    return evt.get_function_name();
}

pattern_formatter::end_of_line_piece::end_of_line_piece(justification just,
                                                        std::size_t min_width,
                                                        std::size_t max_width)
    : piece(just, min_width, max_width)
{
    std::ostringstream stream;
    stream << std::endl;
    eol_ = stream.str();
}

std::string pattern_formatter::end_of_line_piece::get_text_impl(const event& evt) const
{
    return eol_;
}

pattern_formatter::level_piece::level_piece(justification just,
                                            std::size_t min_width,
                                            std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::level_piece::get_text_impl(const event& evt) const
{
    return evt.get_logger()->get_effective_level()->get_name();
}

pattern_formatter::milliseconds_since_start_piece::milliseconds_since_start_piece(justification just,
                                                                                  std::size_t min_width,
                                                                                  std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

pattern_formatter::pid_piece::pid_piece(justification just,
                                        std::size_t min_width,
                                        std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

pattern_formatter::literal_piece::literal_piece(const std::string& text)
    : piece(justification::RIGHT, 0, std::numeric_limits<std::size_t>::max()),
      text_(text)
{
}

std::string pattern_formatter::literal_piece::get_text_impl(const event& evt) const
{
    return text_;
}

pattern_formatter::thread_piece::thread_piece(justification just,
                                              std::size_t min_width,
                                              std::size_t max_width)
    : piece(just, min_width, max_width)
{
}

std::string pattern_formatter::thread_piece::get_text_impl(const event& evt) const
{
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    return stream.str();
}

}
