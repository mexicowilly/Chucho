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

#include <chucho/file_roll_trigger.hpp>
#include <chrono>

namespace chucho
{

class CHUCHO_EXPORT interval_file_roll_trigger : public file_roll_trigger
{
public:
    typedef std::chrono::system_clock clock_type;

    enum class period : std::size_t
    {
        MINUTE,
        HOUR,
        DAY,
        WEEK,
        MONTH
    };

    interval_file_roll_trigger(period prd, std::size_t count);

    clock_type::time_point get_next_roll() const;
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
