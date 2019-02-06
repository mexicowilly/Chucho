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

#if !defined(CHUCHO_TIME_FILE_ROLLER_HPP__)
#define CHUCHO_TIME_FILE_ROLLER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/file_roller.hpp>
#include <chucho/file_roll_trigger.hpp>
#include <chucho/optional.hpp>
#include <chrono>
#include <set>

namespace chucho
{

/**
 * @class time_file_roller time_file_roller.hpp chucho/time_file_roller.hpp 
 * A @ref file_roller that rolls files by time. The roller 
 * operates by taking a file name pattern in its constructor. 
 * This pattern has a date specification that is similar to the 
 * UTC date specification accepted by @ref pattern_formatter, 
 * except that here the "%q" field for milliseconds is not 
 * accepted. For example, a file name pattern might be 
 * "/var/log/my.log.%d{%Y.%m.%d}". Only the UTC date 
 * specification that begins with "%d" is accepted. The local 
 * time specification that begins with "%D" does not count. 
 *  
 * The roller determines the rolling period by examining the 
 * date specification. In the case of 
 * "/var/log/my.log.%d{%Y.%m.%d}", the file would roll daily at 
 * the beginning of each day, since the shortest time period 
 * expressed in the date specification is day. 
 *  
 * More than one date specification may appear in the file name 
 * pattern, but only one is considered the primary 
 * specification. Any others must be tagged with ", aux", as in 
 * "/var/%d{%Y.%m, aux}/%d{%d}". Note here that the auxillary 
 * date specification is a directoy name in the file name 
 * pattern. This is permitted. 
 *  
 * The time_file_roller also controls the number of files that 
 * are rolled over. A maximum history parameter is accepted in 
 * the constructor, and any files that fall outside that history 
 * window are removed. Empty directories are never removed, but 
 * regular files that fall outside the window are. 
 *  
 * @note The time_file_roller is also a @ref file_roll_trigger, 
 * so it is not necessary to use an additional trigger when 
 * using a time_file_roller. 
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT time_file_roller : public file_roller,
                                       public file_roll_trigger
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a time_file_roller.
     * 
     * @param file_name_pattern the file name pattern
     * @param max_history the maximum number of files to be retained 
     *                    in the rolling history
     * @param cmp the @ref file_compressor
     */
    time_file_roller(const std::string& file_name_pattern,
                     std::size_t max_history,
                     std::unique_ptr<file_compressor>&& cmp = std::move(std::unique_ptr<file_compressor>()));
    //@}

    virtual std::string get_active_file_name() override;
    /**
     * Return the file name pattern that this roller is using.
     * 
     * @return the file name pattern
     */
    const std::string& get_file_name_pattern() const;
    /**
     * Return the maximum history retained by the roller.
     * 
     * @return the maximum history
     */
    std::size_t get_max_history() const;
    virtual bool is_triggered(const std::string& active_file, const event& e) override;
    virtual void roll() override;

private:
    typedef std::chrono::system_clock clock_type;
    typedef std::chrono::time_point<clock_type> time_type;

    enum class period : std::size_t
    {
        UNKNOWN,
        MINUTE = 60,
        HOUR = 60 * 60,             // Sun's compiler can't refer to MINUTE here
        DAY = 60 * 60 * 24,         // Nor HOUR here
        WEEK = 60 * 60 * 24 * 7,    // Nor DAY here
        MONTH,
        YEAR
    };

    class CHUCHO_NO_EXPORT cleaner : public status_reporter
    {
    public:
        cleaner(time_file_roller& roller);

        void clean(const time_type& now, const std::string& active_file_name);

    private:
        void clean_one(const time_type& t,
                       int period_offset,
                       std::set<std::string>& cleaned,
                       const std::string& active_file_name);
        std::size_t periods_elapsed(const time_type& first, const time_type& last);
        std::size_t periods_since_last(const time_type& now);

        time_file_roller& roller_;
        int oldest_period_offset_;
        optional<time_type> last_clean_;
    };

    CHUCHO_NO_EXPORT void compute_next_roll(const time_type& now);
    CHUCHO_NO_EXPORT std::string find_time_spec(const std::string& str,
                                                std::size_t& start,
                                                std::size_t& end) const;
    CHUCHO_NO_EXPORT time_type relative(const time_type& t, int period_offset);
    CHUCHO_NO_EXPORT std::string resolve_file_name(const time_type& tm) const;
    CHUCHO_NO_EXPORT void set_period();

    time_type next_roll_;
    std::size_t max_history_;
    std::string file_name_pattern_;
    period period_;
    std::unique_ptr<cleaner> cleaner_;
};

inline const std::string& time_file_roller::get_file_name_pattern() const
{
    return file_name_pattern_;
}

inline std::size_t time_file_roller::get_max_history() const
{
    return max_history_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
