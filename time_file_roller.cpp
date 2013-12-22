/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/time_file_roller.hpp>
#include <chucho/calendar.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/regex.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <algorithm>
#include <array>
#include <iomanip>
#include <sstream>

namespace
{

const std::size_t SECONDS_PER_DAY(86400);
// Only look for files to delete that are within 14 * 24 periods old.
// (This constant is borrowed from logback.)
const std::size_t MAX_INACTIVE_PERIODS(14 * 24);

struct static_data
{
    static_data();

    chucho::regex::expression aux_time_spec_re_;
    // Only look for files to delete that are less than 64 days old, in case
    // we don't know when last time we checked was.
    // (This constant is borrowed from logback.)
    const std::chrono::seconds MAX_PAST_SEARCH;
};

static_data::static_data()
    : aux_time_spec_re_(" *, *aux *$"),
      MAX_PAST_SEARCH(64 * SECONDS_PER_DAY)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
    // This gets cleaned in finalize()
    static static_data* dt;

    std::call_once(once, [&] () { dt = new static_data(); });
    return *dt;
}

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

time_file_roller::time_file_roller(const std::string& file_name_pattern,
                                   std::size_t max_history,
                                   std::shared_ptr<file_compressor> cmp)
    : file_roller(cmp),
      max_history_(max_history),
      file_name_pattern_(file_name_pattern)
{
    set_status_origin("time_file_roller");
    set_period();
    cleaner_.reset(new cleaner(*this));
    compute_next_roll(clock_type::now());
}

void time_file_roller::compute_next_roll(const time_type& now)
{
    calendar::pieces next_cal = calendar::get_utc(clock_type::to_time_t(now));
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
                                             std::size_t& end) const
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

std::string time_file_roller::get_active_file_name()
{
    return file_writer_->get_initial_file_name().empty() ?
        resolve_file_name(clock_type::now()) : file_writer_->get_initial_file_name();
}

bool time_file_roller::is_triggered(const std::string& active_file, const event& e)
{
    calendar::pieces now_cal = calendar::get_utc(clock_type::to_time_t(clock_type::now()));
    time_type now = clock_type::from_time_t(std::mktime(&now_cal));
    return now >= next_roll_;
}

time_file_roller::time_type time_file_roller::relative(const time_type& t, int period_offset)
{
    std::time_t st = clock_type::to_time_t(t);
    calendar::pieces cal = calendar::get_local(st);
    if (period_ == period::MINUTE)
        cal.tm_min += period_offset;
    else if (period_ == period::HOUR)
        cal.tm_hour += period_offset;
    else if (period_ == period::DAY)
        cal.tm_mday += period_offset;
    else if (period_ == period::WEEK)
        cal.tm_mday += period_offset * 7;
    else if (period_ == period::MONTH)
        cal.tm_mon += period_offset;
    else if (period_ == period::YEAR)
        cal.tm_year += period_offset;
    st = std::mktime(&cal);
    return clock_type::from_time_t(st);
}

std::string time_file_roller::resolve_file_name(const time_type& tm) const
{
    calendar::pieces cal = calendar::get_utc(clock_type::to_time_t(tm));
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
            spec = regex::replace(spec, data().aux_time_spec_re_, std::string());
            std::string fmt = calendar::format(cal, spec);
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
        try
        {
            file::remove(target);
        }
        catch (...)
        {
            // don't care
        }
        std::rename(get_active_file_name().c_str(), target.c_str());
        if (compressor_)
        {
            int cmp = -static_cast<int>(compressor_->get_min_index());
            // If the writer has an active file name set, then we
            // set the compression index to one step closer to the
            // active.
            if (!file_writer_->get_initial_file_name().empty())
                cmp++;
            std::string to_compress = resolve_file_name(relative(now, cmp));
            if (file::exists(to_compress))
                compressor_->compress(to_compress);
        }
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
        if (regex::search(spec, data().aux_time_spec_re_))
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
            calendar::pieces epoch = calendar::get_utc(0);
            std::string fmt1 = calendar::format(epoch, spec);
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
                calendar::pieces rolled = epoch;
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
                std::string fmt2 = calendar::format(rolled, spec);
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
      oldest_period_offset_(-static_cast<int>(roller.max_history_) - 1)
{
    set_status_origin("time_file_roller::cleaner");
}

void time_file_roller::cleaner::clean(const time_type& now, const std::string& active_file_name)
{
    std::size_t periods = periods_since_last(now);
    if (periods > 1)
        report_info(std::to_string(periods) + " periods have elapsed since last clean");
    std::set<std::string> cleaned;
    int actual_oldest = roller_.file_writer_->get_initial_file_name().empty() ?
        oldest_period_offset_ : oldest_period_offset_ + 1;
    for (std::size_t i = 0; i < periods; i++)
        clean_one(now, actual_oldest - i, cleaned, active_file_name);
    last_clean_ = now;
}

void time_file_roller::cleaner::clean_one(const time_type& t,
                                          int period_offset,
                                          std::set<std::string>& cleaned,
                                          const std::string& active_file_name)
{
    time_type rel = roller_.relative(t, period_offset);
    std::string name = roller_.resolve_file_name(rel);
    if (roller_.compressor_)
    {
        if (static_cast<unsigned>(std::abs(period_offset)) >= roller_.compressor_->get_min_index())
            name += roller_.compressor_->get_extension();
    }
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
        calendar::pieces first_cal = calendar::get_utc(clock_type::to_time_t(first));
        calendar::pieces last_cal = calendar::get_utc(clock_type::to_time_t(last));
        int year_diff = last_cal.tm_year - first_cal.tm_year;
        int month_diff = last_cal.tm_mon - first_cal.tm_mon;
        result = year_diff * 12 + month_diff;
    }
    else if (roller_.period_ == period::YEAR)
    {
        calendar::pieces first_cal = calendar::get_utc(clock_type::to_time_t(first));
        calendar::pieces last_cal = calendar::get_utc(clock_type::to_time_t(last));
        result = last_cal.tm_year - first_cal.tm_year;
    }
    else
    {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(last - first);
        result = static_cast<std::size_t>(diff.count() / static_cast<long long>(roller_.period_));
    }
    return result;
}

std::size_t time_file_roller::cleaner::periods_since_last(const time_type& now)
{
    if (roller_.period_ == period::UNKNOWN)
        throw std::invalid_argument("Cannot compute periods since last clean with unknown period type");
    return last_clean_ ?
         periods_elapsed(*last_clean_, now) :
         std::min(periods_elapsed(now - data().MAX_PAST_SEARCH, now), MAX_INACTIVE_PERIODS);
}

}
