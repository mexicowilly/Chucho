/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_EMAIL_TRIGGER_HPP__

#include <chucho/email_trigger.hpp>
#include <chucho/level.hpp>

namespace chucho
{

/**
 * @class level_threshold_email_trigger level_threshold_email_trigger.hpp chucho/level_threshold_email_trigger.hpp
 * An email trigger that fires whenever an event reaches a certain
 * log level. If the @ref level of the event is greater than or
 * equal to the @ref level of the trigger, then the trigger fires.
 * 
 * @ingroup email
 */
class CHUCHO_EXPORT level_threshold_email_trigger : public email_trigger
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a trigger.
     * 
     * @param lvl the level to which event levels will be compared
     */
    level_threshold_email_trigger(std::shared_ptr<level> lvl);
    /**
     * @}
     */

    /**
     * Return the level of this trigger.
     * 
     * @return the level
     */
    std::shared_ptr<level> get_level() const;
    /**
     * Return true if the @ref level of evt is greater than or
     * equal to the @ref level of this trigger.
     * 
     * @param evt the event to examine
     * @return true if the level of the event is greater than or
     * equal to the level of this trigger
     */
    virtual bool is_triggered(const event& evt) override;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_email_trigger::get_level() const
{
    return level_;
}

}

#endif

