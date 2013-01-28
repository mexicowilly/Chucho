#include <chucho/time_file_roller.hpp>
#include <chucho/calendar.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <chucho/file_writer.hpp>
#include <regex>
#include <algorithm>
#include <array>

namespace
{

const std::regex aux_time_spec_re(" *, *aux *$");
const std::size_t SECONDS_PER_DAY(86400);
// Only look for files to delete that are less than 64 days old, in case
// we don't know when last time we checked was.
// (This constant is borrowed from logback.)
const std::chrono::seconds MAX_PAST_SEARCH(64 * SECONDS_PER_DAY);
// Only look for files to delete that are within 14 * 24 periods old.
// (This constant is borrowed from logback.)
const std::size_t MAX_INACTIVE_PERIODS(14 * 24);

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

}

namespace chucho
{

time_file_roller::time_file_roller(const std::string file_name_pattern,
                                   std::size_t max_history)
    : max_history_(max_history),
      file_name_pattern_(file_name_pattern)
{
    set_status_origin("time_file_roller");
    set_period();
    cleaner_.reset(new cleaner(*this));
    compute_next_roll(clock_type::now());
}

void time_file_roller::compute_next_roll(const time_type& now)
{
    struct std::tm next_cal = calendar::get_utc(clock_type::to_time_t(now));
    while (now >= next_roll_)
    {
        if (period_ == period::MINUTE)
        {
            next_cal.tm_min++;
            next_cal.tm_sec = 0;
        }
        else if (period_ == period::HOUR)
        {
            next_cal.tm_hour++;
            next_cal.tm_min = 0;
            next_cal.tm_sec = 0;
        }
        else if (period_ == period::DAY)
        {
            next_cal.tm_mday++;
            next_cal.tm_hour = 0;
            next_cal.tm_min = 0;
            next_cal.tm_sec = 0;
        }
        else if (period_ == period::WEEK)
        {
            next_cal.tm_mday += 7;
            next_cal.tm_hour = 0;
            next_cal.tm_min = 0;
            next_cal.tm_sec = 0;
        }
        else if (period_ == period::MONTH)
        {
            next_cal.tm_mon++;
            next_cal.tm_mday = 1;
            next_cal.tm_hour = 0;
            next_cal.tm_min = 0;
            next_cal.tm_sec = 0;
        }
        else if (period_ == period::YEAR)
        {
            next_cal.tm_year++;
            next_cal.tm_mon = 0;
            next_cal.tm_mday = 1;
            next_cal.tm_hour = 0;
            next_cal.tm_min = 0;
            next_cal.tm_sec = 0;
        }
        next_roll_ = clock_type::from_time_t(std::mktime(&next_cal));
    }
}

std::string time_file_roller::find_time_spec(const std::string& str,
                                             std::size_t& start,
                                             std::size_t& end)
{
    std::string result;
    start = find_time_token(str, 'd', start);
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
            {
                report_error("Expected '}' in the file name pattern");
                result = "%Y-%m-%d";
                end = found;
            }
            else
            {
                result = str.substr(found, end - found);
                if (++end >= str.length())
                    end = std::string::npos;
            }
        }
    }
    return result;
}

std::string time_file_roller::format(const struct std::tm& cal, const std::string& spec)
{
    std::string result;
    std::array<char, 1024> buf;
    std::size_t count = std::strftime(buf.data(), buf.size(), spec.c_str(), &cal);
    if (count == 0)
        report_error("Unable to parse date format " + spec);
    else
        result.assign(buf.data(), count);
    return result;
}

std::string time_file_roller::get_active_file_name()
{
    return file_writer_->get_initial_file_name().empty() ?
        resolve_file_name(clock_type::now()) : file_writer_->get_initial_file_name();
}

bool time_file_roller::is_triggered(const std::string& active_file, const event& e)
{
    struct std::tm now_cal = calendar::get_utc(clock_type::to_time_t(clock_type::now()));
    time_type now = clock_type::from_time_t(std::mktime(&now_cal));
    return now >= next_roll_;
}

std::string time_file_roller::resolve_file_name(const time_type& tm)
{
    std::tm cal = calendar::get_utc(clock_type::to_time_t(tm));
    std::size_t start;
    std::size_t end;
    std::string result = file_name_pattern_;
    bool got_one = false;
    while (true)
    {
        start = 0;
        std::string spec = find_time_spec(result, start, end);
        if (start == std::string::npos)
        {
            if (!got_one)
                report_error("The file name pattern " + file_name_pattern_ + " contains no date specification");
            break;
        }
        else
        {
            got_one = true;
            spec = std::regex_replace(spec, aux_time_spec_re, "");
            std::string fmt = format(cal, spec);
            if (!fmt.empty())
                result.replace(start, end - start, fmt);
        }
    }
    if (file::dir_sep == '\\')
        std::replace(result.begin(), result.end(), '/', '\\');
    else
        std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}

void time_file_roller::roll()
{
    if (period_ == period::UNKNOWN)
    {
        report_error("The file cannot roll because the rolling time period is unknown");
    }
    else
    {
        time_type now = clock_type::now();
        std::string target = resolve_file_name(now);
        if (file::exists(target))
            file::remove(target);
        std::rename(get_active_file_name().c_str(), target.c_str());
        compute_next_roll(now);
        cleaner_->clean(now, get_active_file_name());
    }
}

void time_file_roller::set_period()
{
    std::size_t pos = 0;
    std::size_t end;
    period_ = period::UNKNOWN;
    bool found_aux = false;
    std::string primary_spec;
    do
    {
        std::string spec = find_time_spec(file_name_pattern_, pos, end);
        if (spec.empty())
            break;
        pos = end;
        if (std::regex_search(spec, aux_time_spec_re))
        {
            found_aux = true;
        }
        else
        {
            if (!primary_spec.empty())
            {
                report_error("The file name pattern " + file_name_pattern_ + " can have only one primary date specification. The others must be marked with \",aux\". Found " + primary_spec + " and " + spec + ".");
                return;
            }
            primary_spec = spec;
            struct std::tm epoch = calendar::get_utc(0);
            std::string fmt1 = format(epoch, spec);
            if (fmt1.empty())
            {
                report_error("The date specification \"" + spec + "\" cannot be used to format a date");
                return;
            }
            std::array<period, 6> periods =
            {
                period::MINUTE,
                period::HOUR,
                period::DAY,
                period::WEEK,
                period::MONTH,
                period::YEAR
            };
            for (period p : periods)
            {
                struct std::tm rolled = epoch;
                if (p == period::MINUTE)
                    rolled.tm_min++;
                else if (p == period::HOUR)
                    rolled.tm_hour++;
                else if (p == period::DAY)
                    rolled.tm_mday++;
                else if (p == period::WEEK)
                    rolled.tm_mday += 7;
                else if (p == period::MONTH)
                    rolled.tm_mon++;
                else
                    rolled.tm_year++;
                rolled = calendar::get_local(std::mktime(&rolled));
                std::string fmt2 = format(rolled, spec);
                if (fmt1 != fmt2) {
                    period_ = p;
                    break;
                }
            }
            if (period_ == period::UNKNOWN)
                report_error("The date specification " + spec + " does not contain sufficient information to determine the rolling period");
        }
    } while (pos != std::string::npos);
    if (period_ == period::UNKNOWN)
    {
        if (found_aux)
            report_error("No non-auxillary date specifications were found in the pattern \"" + file_name_pattern_ + "\"");
        else
            report_error("No suitable date specifications were found in the pattern \"" + file_name_pattern_ + "\"");
    }
}

time_file_roller::cleaner::cleaner(time_file_roller& roller)
    : roller_(roller),
      oldest_period_offset_(-roller.max_history_ - 1)
{
    set_status_origin("time_file_roller::cleaner");
}

void time_file_roller::cleaner::clean(const time_type& now, const std::string& active_file_name)
{
    std::size_t periods = periods_since_last(now);
    if (periods > 1)
        report_info(std::to_string(periods) + " periods have elapsed since last clean");
    std::set<std::string> cleaned;
    for (auto i = 0; i < periods; i++)
        clean_one(now, oldest_period_offset_ - i, cleaned, active_file_name);
    last_clean_ = now;
}

void time_file_roller::cleaner::clean_one(const time_type& t,
                                          int period_offset,
                                          std::set<std::string>& cleaned,
                                          const std::string& active_file_name)
{
    time_type rel = relative(t, period_offset);
    std::string name = roller_.resolve_file_name(rel);
    if (cleaned.count(name) == 0 &&
        name != active_file_name &&
        file::exists(name))
    {
        report_info("Removing " + name);
        file::remove(name);
        cleaned.insert(name);
    }
}

std::size_t time_file_roller::cleaner::periods_elapsed(const time_type& first, const time_type& last)
{
    if (first > last)
        throw std::invalid_argument("The starting time must be before the ending time");
    std::size_t result;
    if (roller_.period_ == period::MONTH)
    {
        struct std::tm first_cal = calendar::get_utc(clock_type::to_time_t(first));
        struct std::tm last_cal = calendar::get_utc(clock_type::to_time_t(last));
        int year_diff = last_cal.tm_year - first_cal.tm_year;
        int month_diff = last_cal.tm_mon - first_cal.tm_mon;
        result = year_diff * 12 + month_diff;
    }
    else if (roller_.period_ == period::YEAR)
    {
        struct std::tm first_cal = calendar::get_utc(clock_type::to_time_t(first));
        struct std::tm last_cal = calendar::get_utc(clock_type::to_time_t(last));
        result = last_cal.tm_year - first_cal.tm_year;
    }
    else
    {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(last - first);
        result = diff.count() / static_cast<long long>(roller_.period_);
    }
    return result;
}

std::size_t time_file_roller::cleaner::periods_since_last(const time_type& now)
{
    if (roller_.period_ == period::UNKNOWN)
        throw std::invalid_argument("Cannot compute periods since last clean with unknown period type");
    return last_clean_ ?
         periods_elapsed(*last_clean_, now) :
         std::min(periods_elapsed(now - MAX_PAST_SEARCH, now), MAX_INACTIVE_PERIODS);
}

time_file_roller::time_type time_file_roller::cleaner::relative(const time_type& t, int period_offset)
{
    std::time_t st = clock_type::to_time_t(t);
    struct std::tm cal = calendar::get_local(st);
    if (roller_.period_ == period::MINUTE)
        cal.tm_min += period_offset;
    else if (roller_.period_ == period::HOUR)
        cal.tm_hour += period_offset;
    else if (roller_.period_ == period::DAY)
        cal.tm_mday += period_offset;
    else if (roller_.period_ == period::WEEK)
        cal.tm_mday += period_offset * 7;
    else if (roller_.period_ == period::MONTH)
        cal.tm_mon += period_offset;
    else if (roller_.period_ == period::YEAR)
        cal.tm_year += period_offset;
    st = std::mktime(&cal);
    return clock_type::from_time_t(st);
}

}
