/*
 * Copyright 2013-2019 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/calendar.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <chucho/marker.hpp>
#include <chucho/diagnostic_context.hpp>
#include <chucho/line_ending.hpp>
#include <chucho/host.hpp>
#include <chucho/time_util.hpp>
#include <chucho/process.hpp>
#include <chucho/regex.hpp>
#include <limits>
#include <sstream>
#include <mutex>
#include <thread>
#include <cstdio>
#include <iomanip>
#include <algorithm>
#include <cassert>

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

std::string::const_iterator find(std::string::const_iterator begin,
                                 std::string::const_iterator end,
                                 char ch)
{
    for ( ; begin != end; ++begin)
    {
        if (*begin == ch)
            return begin;
    }
    return end;
}

}

namespace chucho
{

pattern_formatter::pattern_formatter(const std::string& pattern)
{
    set_status_origin("pattern_formatter");
    parse(pattern);
}

std::unique_ptr<pattern_formatter::piece> pattern_formatter::create_piece(std::string::const_iterator& pos,
                                                                          std::string::const_iterator end,
                                                                          const format_params& params)
{
    std::unique_ptr<piece> result;
    std::string arg;
    char c = *pos;
    switch (c)
    {
    case 'b':
        result = std::make_unique<base_file_piece>(params);
        break;
    case 'c':
        arg = get_argument(pos, end);
        result = std::make_unique<logger_piece>(arg, params);
        break;
    case 'C':
        arg = get_argument(pos, end);
        if (arg.empty())
            report_error("The pattern parameter %C requires an argument");
        else
            result = std::make_unique<diagnostic_context_piece>(arg, params);
        break;
    case 'd':
    case 'D':
        arg = get_argument(pos, end);
        if (arg.empty())
            arg = "%Y-%m-%d %H:%M:%S";
        if (c == 'd')
            result = std::make_unique<utc_date_time_piece>(arg, params);
        else
            result = std::make_unique<local_date_time_piece>(arg, params);
        break;
    case 'F':
        result = std::make_unique<full_file_piece>(params);
        break;
    case 'h':
        result = std::make_unique<base_host_piece>(params);
        break;
    case 'H':
        result = std::make_unique<full_host_piece>(params);
        break;
    case 'i':
        result = std::make_unique<pid_piece>(params);
        break;
    case 'k':
        result = std::make_unique<marker_piece>(params);
        break;
    case 'L':
        result = std::make_unique<line_number_piece>(params);
        break;
    case 'm':
        result = std::make_unique<message_piece>(params);
        break;
    case 'M':
        result = std::make_unique<function_piece>(params);
        break;
    case 'n':
        result = std::make_unique<end_of_line_piece>(params);
        break;
    case 'p':
        result = std::make_unique<level_piece>(params);
        break;
    case 'r':
        result = std::make_unique<milliseconds_since_start_piece>(params);
        break;
    case 'R':
        arg = get_argument(pos, end);
        if (arg.empty())
            report_error("The pattern parameter %R requires three parameters enclosed in curly braces and separated by commas");
        else
            result = std::make_unique<regex_replace_piece>(arg, params);
        break;
    case 't':
        result = std::make_unique<thread_piece>(params);
        break;
    default:
        report_error(std::string("Unexpected character '") + c + "' in pattern");
        throw exception("Invalid parameter");
    }
    return std::move(result);
}

std::string pattern_formatter::format(const event& evt)
{
    std::string result;
    for (auto& p : pieces_)
        result += p->get_text(evt);
    return result;
}

std::string pattern_formatter::get_argument(std::string::const_iterator& pos,
                                            std::string::const_iterator end)
{
    std::string result;
    if (pos != end && (pos + 1) != end && *(pos + 1) == '{')
    {
        // std::find does not work under g++ 4.7
        auto last = find(++pos, end, '}');
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
                            pieces_.emplace_back(std::make_unique<literal_piece>(text));
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
                        pieces_.push_back(std::move(create_piece(i, pattern.end(), params)));
                        state = parser_state::LITERAL;
                        text.clear();
                        params.reset();
                    }
                }
                break;
            case parser_state::MIN:
                if (*i >= '0' && *i <= '9')
                {
                    text += *i;
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
                        pieces_.push_back(std::move(create_piece(i, pattern.end(), params)));
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
                    pieces_.push_back(std::move(create_piece(i, pattern.end(), params)));
                    state = parser_state::LITERAL;
                    text.clear();
                    params.reset();
                }
                break;
            }
        }
        catch (...)
        {
            pieces_.emplace_back(std::make_unique<literal_piece>(std::string(begin_pos, i + 1)));
            state = parser_state::LITERAL;
            text.clear();
            params.reset();
        }
    }
    if (!text.empty())
    {
        pieces_.emplace_back(std::make_unique<literal_piece>(text));
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

pattern_formatter::logger_piece::logger_piece(const std::string& num, const format_params& params)
    : piece(params),
      count_(std::numeric_limits<std::size_t>::max())
{
    try
    {
        if (!num.empty())
            count_ = std::stoul(num);
    }
    catch (...)
    {
    }
}

std::string pattern_formatter::logger_piece::get_text_impl(const event& evt) const
{
    std::string nm = evt.get_logger()->get_name();
    auto hrchy_count = std::count(nm.begin(), nm.end(), '.') + 1;
    if (hrchy_count > 1 && count_ <= hrchy_count)
    {
        auto pos = 0;
        for (auto i = 0; i < hrchy_count - count_; i++)
            pos = nm.find('.', pos) + 1;
        if (pos > 0 && pos < nm.length() - 1)
            nm.erase(0, pos);
    }
    return nm;
}

pattern_formatter::date_time_piece::date_time_piece(const std::string& date_pattern,
                                                    const format_params& params,
                                                    int location)
    : piece(params),
      fmt_(std::make_unique<calendar::formatter>(date_pattern,
                                                 static_cast<calendar::formatter::location>(location)))
{
}

std::string pattern_formatter::date_time_piece::get_text_impl(const event& evt) const
{
    return fmt_->format(evt.get_time().time_since_epoch());
}

pattern_formatter::utc_date_time_piece::utc_date_time_piece(const std::string& date_pattern,
                                                            const format_params& params)
    : date_time_piece(date_pattern, params, calendar::formatter::UTC)
{
}

pattern_formatter::local_date_time_piece::local_date_time_piece(const std::string& date_pattern,
                                                                const format_params& params)
    : date_time_piece(date_pattern, params, calendar::formatter::LOCAL)
{
}

pattern_formatter::base_host_piece::base_host_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::base_host_piece::get_text_impl(const event& evt) const
{
    return host::get_base_name();
}

pattern_formatter::full_host_piece::full_host_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::full_host_piece::get_text_impl(const event& evt) const
{
    return host::get_full_name();
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
}

std::string pattern_formatter::end_of_line_piece::get_text_impl(const event& evt) const
{
    return line_ending::EOL;
}

pattern_formatter::level_piece::level_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::level_piece::get_text_impl(const event& evt) const
{
    return evt.get_level()->get_name();
}

pattern_formatter::milliseconds_since_start_piece::milliseconds_since_start_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::milliseconds_since_start_piece::get_text_impl(const event& evt) const
{
    return std::to_string(time_util::milliseconds_since_start());
}

pattern_formatter::pid_piece::pid_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::pid_piece::get_text_impl(const event& evt) const
{
    return std::to_string(process::id());
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
    if (evt.get_thread_id())
    {
        return *evt.get_thread_id();
    }
    else
    {
        std::ostringstream stream;
        stream << std::this_thread::get_id();
        return stream.str();
    }
}

pattern_formatter::marker_piece::marker_piece(const format_params& params)
    : piece(params)
{
}

std::string pattern_formatter::marker_piece::get_text_impl(const event& evt) const
{
    std::ostringstream stream;
    if (evt.get_marker())
        stream << *evt.get_marker();
    return stream.str();
}

pattern_formatter::diagnostic_context_piece::diagnostic_context_piece(const std::string& key,
                                                                      const format_params& params)
    : piece(params),
      key_(key)
{
}

std::string pattern_formatter::diagnostic_context_piece::get_text_impl(const event& evt) const
{
    return diagnostic_context::at(key_);
}

pattern_formatter::regex_replace_piece::regex_replace_piece(const std::string& args,
                                                            const format_params& params)
    : piece(params)
{
    regex::expression arg_re("^ *\"([^\"]+)\" *, *\"([^\"]+)\" *, *\"([^\"]*)\" *$");
    regex::match mch;
    if (regex::search(args, arg_re, mch))
    {
        assert(mch.size() == 4);
        fmt_ = std::make_unique<pattern_formatter>(args.substr(mch[1].begin(), mch[1].length()));
        re_ = std::make_unique<regex::expression>(args.substr(mch[2].begin(), mch[2].length()));
        replacement_ = args.substr(mch[3].begin(), mch[3].length());
    }
    else
    {
        throw exception("Invalid replacement arguments: '" + args + "'");
    }
}

std::string pattern_formatter::regex_replace_piece::get_text_impl(const event& evt) const
{
    return regex::replace(fmt_->format(evt), *re_, replacement_);
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
