/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_HPP__)
#define CHUCHO_INTERVAL_FILE_ROLL_TRIGGER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/file_roll_trigger.hpp>
#include <chrono>

namespace chucho
{

/**
 * @class interval_file_roll_trigger interval_file_roll_trigger.hpp chucho/interval_file_roll_trigger.hpp 
 * A @ref file_roll_trigger that will be triggered at intervals 
 * of time. You may select any number of minutes, hours, days, 
 * weeks or months as the interval. 
 * 
 * @ingroup rolling
 */
class CHUCHO_EXPORT interval_file_roll_trigger : public file_roll_trigger
{
public:
    /**
     * The type of clock used to measure time.
     */
    typedef std::chrono::system_clock clock_type;

    /**
     * Time periods at which a roll can occur.
     */
    enum class period
    {
        MINUTE, /**< A minute */
        HOUR,   /**< An hour */
        DAY,    /**< A day */
        WEEK,   /**< A week */
        MONTH   /**< A month */
    };

    /**
     * @name Constructor
     * @{ 
     */
    /**
     * Construct an interval file roll trigger. Files will roll 
     * after count number of periods have elapsed. 
     * 
     * @param prd the period of time
     * @param count the number of periods to wait before triggering
     */
    interval_file_roll_trigger(period prd, std::size_t count);
    /** @} */

    /**
     * Return the time at which the next roll is scheduled to occur.
     * 
     * @return the time
     */
    clock_type::time_point get_next_roll() const;
    /**
     * If the current time is after the scheduled next roll time, 
     * then this trigger fires.
     * 
     * @param active_file the currently active file name
     * @param e the log event
     * @return true if the current time is after the scheduled time
     */
    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    void CHUCHO_NO_EXPORT compute_next_roll();

    clock_type::time_point next_roll_;
    period period_;
    std::size_t count_;
};

inline interval_file_roll_trigger::clock_type::time_point interval_file_roll_trigger::get_next_roll() const
{
    return next_roll_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
