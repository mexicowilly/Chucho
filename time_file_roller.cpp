#include <chucho/time_file_roller.hpp>
#include <chucho/calendar.hpp>
#include <chucho/file.hpp>
#include <regex>
#include <algorithm>
#include <array>

namespace
{

std::basic_regex<char> aux_time_spec_re(" *, *aux *$");

std::size_t find_time_token(const std::string& str, char tok, std::size_t pos = 0)
{
    char full_tok[3] = { '%', tok, 0 };
    do
    {
        pos = str.find(full_tok, pos);
        if (pos == std::string::npos)
            return pos;
    } while (pos != 0 && str[pos - 1] == '%');
    return pos;
}

std::string find_time_spec(const std::string& str, std::size_t& start, std::size_t& end)
{
    std::string result;
    start = find_time_token(str, 'D');
    if (start == std::string::npos)
    {
        end = std::string::npos;
    }
    else
    {
        auto found = start + 2;
        if (found == str.length() || str[found] != '{')
        {
            result = "%Y-%m-%d";
            end = found;
        }
        else
        {
            end = str.find('}', ++found);
            if (end == std::string::npos)
                throw chucho::time_file_roller_exception("Expected '}' in the file name pattern");
            result = str.substr(found, end - found);
            ++end;
        }
    }
    return result;
}

}

namespace chucho
{

time_file_roller_exception::time_file_roller_exception(const std::string& msg)
    : exception(msg)
{
}

void time_file_roller::compute_next_roll(const time_type& now)
{
    struct std::tm next_cal = calendar::get_local(clock_type::to_time_t(next_roll_));
    while (now >= next_roll_)
    {
        if (period_ == period::HOUR)
            next_cal.tm_hour++;
        else if (period_ == period::DAY)
            next_cal.tm_mday++;
        else if (period_ == period::MONTH)
            next_cal.tm_mon++;
        else if (period_ == period::YEAR)
            next_cal.tm_year++;
        next_roll_ = clock_type::from_time_t(std::mktime(&next_cal));
    }
}

std::string time_file_roller::get_active_file_name()
{
    return std::string();
}

bool time_file_roller::is_triggered(const std::string& active_file, const event& e)
{
    return clock_type::now() >= next_roll_;
}

std::string time_file_roller::resolve_file_name(const time_type& tm)
{
    std::tm cal = calendar::get_local(clock_type::to_time_t(tm));
    std::size_t start;
    std::size_t end;
    std::string result = file_name_pattern_;
    std::array<char, 1024> time_buf;
    do
    {
        start = 0;
        std::string spec = find_time_spec(result, start, end);
        std::regex_replace(spec, aux_time_spec_re, "");
        if (std::strftime(time_buf.data(), time_buf.size(), spec.c_str(), &cal) == 0)
            throw time_file_roller_exception("Unable to parse date format " + spec);
        result.replace(start, end - start, time_buf.data());
    } while (start != std::string::npos);
    if (file::dir_sep == '\\')
        std::replace(result.begin(), result.end(), '/', '\\');
    else
        std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}

void time_file_roller::roll()
{
}

void time_file_roller::set_period()
{
    std::size_t pos = 0;
    std::size_t end;
    period_ = period::UNKNOWN;
    bool found_aux = false;
    do
    {
        std::string spec = find_time_spec(file_name_pattern_, pos, end);
        if (std::regex_search(spec, aux_time_spec_re))
        {
            found_aux = true;
        }
        else
        {
            if (find_time_token(spec, 'H') != std::string::npos)
            {
                period_ = period::HOUR;
            }
            else if (find_time_token(spec, 'd') != std::string::npos)
            {
                period_ = period::DAY;
            }
            else if (find_time_token(spec, 'm') != std::string::npos)
            {
                period_ = period::MONTH;
            }
            else if (find_time_token(spec, 'y') != std::string::npos ||
                     find_time_token(spec, 'Y') != std::string::npos)
            {
                period_ = period::YEAR;
            }
            else
            {
                throw time_file_roller_exception("The rolling period cannot be deduced from the date specification " + spec);
            }
        }
    } while (pos != std::string::npos);
    if (period_ == period::UNKNOWN)
    {
        if (found_aux)
            throw time_file_roller_exception("No non-auxillary date specifications were found in the pattern " + file_name_pattern_);
        else
            throw time_file_roller_exception("No date specifications were found in the pattern " + file_name_pattern_);
    }
}

}
