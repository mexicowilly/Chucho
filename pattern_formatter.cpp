#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/calendar.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
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

}

namespace chucho
{

pattern_formatter::pattern_formatter(const std::string& pattern)
{
    parse(pattern);
}

std::shared_ptr<pattern_formatter::piece> pattern_formatter::create_piece(std::string::const_iterator& pos,
                                                                          std::string::const_iterator end,
                                                                          const format_params& params)
{
    std::shared_ptr<piece> result;
    std::string arg;
    char c = *pos;
    switch (c)
    {
    case 'b':
        result.reset(new base_file_piece(params));
        break;
    case 'c':
        result.reset(new logger_piece(params));
        break;
    case 'd':
    case 'D':
        arg = get_argument(pos, end);
        if (arg.empty())
            arg = "%Y-%m-%d %H:%M:%S";
        if (c == 'd')
            result.reset(new utc_date_time_piece(arg, params));
        else
            result.reset(new local_date_time_piece(arg, params));
        break;
    case 'F':
        result.reset(new full_file_piece(params));
        break;
    case 'h':
        result.reset(new base_host_piece(params));
        break;
    case 'i':
        result.reset(new pid_piece(params));
        break;
    case 'L':
        result.reset(new line_number_piece(params));
        break;
    case 'm':
        result.reset(new message_piece(params));
        break;
    case 'M':
        result.reset(new function_piece(params));
        break;
    case 'n':
        result.reset(new end_of_line_piece(params));
        break;
    case 'p':
        result.reset(new level_piece(params));
        break;
    case 'r':
        result.reset(new milliseconds_since_start_piece(params));
        break;
    case 't':
        result.reset(new thread_piece(params));
        break;
    default:
        report_error(std::string("Unexpected character '") + c + "' in pattern");
        throw exception("Invalid parameter");
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
            report_error("Expected '}' in the pattern");
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
    {
        report_error("Empty pattern");
        return;
    }
    parser_state state = parser_state::LITERAL;
    auto last_char = pattern.end() - 1;
    std::string text;
    format_params params;
    std::string::const_iterator begin_pos;
    for (auto i = pattern.begin(); i != pattern.end(); i++)
    {
        try
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
                        begin_pos = i;
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
                    params.just_ = justification::LEFT;
                }
                else
                {
                    if (params.just_ == justification::UNSET)
                        params.just_ = justification::RIGHT;
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
                        pieces_.push_back(create_piece(i, pattern.end(), params));
                        state = parser_state::LITERAL;
                        text.clear();
                        params.reset();
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
                    params.min_width_ = std::stoi(text);
                    if (*i == '.')
                    {
                        state = parser_state::DOT;
                    }
                    else
                    {
                        pieces_.push_back(create_piece(i, pattern.end(), params));
                        state = parser_state::LITERAL;
                        text.clear();
                        params.reset();
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
                    stream << "Invalid pattern: Expected a digit but got '" <<
                        *i << "'";
                    report_error(stream.str());
                    state = parser_state::LITERAL;
                    params.reset();
                    text.assign(begin_pos, i + 1);
                }
                break;
            case parser_state::MAX:
                if (*i >= '0' && *i <= '9')
                {
                    text += *i;
                }
                else
                {
                    params.max_width_ = std::stoi(text);
                    pieces_.push_back(create_piece(i, pattern.end(), params));
                    state = parser_state::LITERAL;
                    text.clear();
                    params.reset();
                }
                break;
            }
        }
        catch (...)
        {
            std::shared_ptr<piece> p(new literal_piece(std::string(begin_pos, i + 1)));
            pieces_.push_back(p);
            state = parser_state::LITERAL;
            text.clear();
            params.reset();
        }
    }
    if (!text.empty())
    {
        std::shared_ptr<piece> p(new literal_piece(text));
        pieces_.push_back(p);
    }
}

pattern_formatter::piece::piece(const format_params& params)
    : params_(params)
{
}

std::string pattern_formatter::piece::get_text(const event& evt) const
{
    std::string result = get_text_impl(evt);
    if (result.length() > params_.max_width_)
    {
        result.erase(result.begin(), result.begin() + (result.length() - params_.max_width_));
    }
    else if (result.length() < params_.min_width_)
    {
        std::size_t pad = params_.min_width_ - result.length();
        if (params_.just_ == justification::LEFT)
            result.append(pad, ' ');
        else
            result.insert(0, pad, ' ');
    }
    return result;
}

pattern_formatter::base_file_piece::base_file_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::base_file_piece::get_text_impl(const event& evt) const
{
    return file::base_name(evt.get_file_name());
}

pattern_formatter::full_file_piece::full_file_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::full_file_piece::get_text_impl(const event& evt) const
{
    return evt.get_file_name();
}

pattern_formatter::logger_piece::logger_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::logger_piece::get_text_impl(const event& evt) const
{
    return evt.get_logger()->get_name();
}

pattern_formatter::date_time_piece::date_time_piece(const std::string& date_pattern,
                                                    const format_params& params)
    : piece(params),
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
                                                            const format_params& params)
    : date_time_piece(date_pattern, params)
{
}

void pattern_formatter::utc_date_time_piece::to_calendar(time_t t, struct std::tm& cal) const
{
    cal = calendar::get_utc(t);
}

pattern_formatter::local_date_time_piece::local_date_time_piece(const std::string& date_pattern,
                                                                const format_params& params)
    : date_time_piece(date_pattern, params)
{
}

void pattern_formatter::local_date_time_piece::to_calendar(time_t t, struct std::tm& cal) const
{
    cal = calendar::get_local(t);
}

std::string pattern_formatter::base_host_piece::get_text_impl(const event& evt) const
{
    return name_;
}

pattern_formatter::line_number_piece::line_number_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::line_number_piece::get_text_impl(const event& evt) const
{
    return std::to_string(evt.get_line_number());
}

pattern_formatter::message_piece::message_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::message_piece::get_text_impl(const event& evt) const
{
    return evt.get_message();
}

pattern_formatter::function_piece::function_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::function_piece::get_text_impl(const event& evt) const
{
    return evt.get_function_name();
}

pattern_formatter::end_of_line_piece::end_of_line_piece(const format_params& params)
    : piece(params)
{
    std::ostringstream stream;
    stream << std::endl;
    eol_ = stream.str();
}

std::string pattern_formatter::end_of_line_piece::get_text_impl(const event& evt) const
{
    return eol_;
}

pattern_formatter::level_piece::level_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::level_piece::get_text_impl(const event& evt) const
{
    return evt.get_logger()->get_effective_level()->get_name();
}

pattern_formatter::milliseconds_since_start_piece::milliseconds_since_start_piece(const format_params& params)
    : piece(params)
{
}

pattern_formatter::pid_piece::pid_piece(const format_params& params)
    : piece(params)
{
}

pattern_formatter::literal_piece::literal_piece(const std::string& text)
    : piece(format_params()),
      text_(text)
{
}

std::string pattern_formatter::literal_piece::get_text_impl(const event& evt) const
{
    return text_;
}

pattern_formatter::thread_piece::thread_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::thread_piece::get_text_impl(const event& evt) const
{
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    return stream.str();
}

pattern_formatter::format_params::format_params()
{
    reset();
}

void pattern_formatter::format_params::reset()
{
    min_width_ = 0;
    max_width_ = std::numeric_limits<std::size_t>::max();
    just_ = justification::UNSET;
}

}
