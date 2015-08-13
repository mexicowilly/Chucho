/*
 * Copyright 2013-2015 Will Mason
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

class CHUCHO_EXPORT level_threshold_email_trigger : public email_trigger
{
public:
    level_threshold_email_trigger(std::shared_ptr<level> lvl);

    std::shared_ptr<level> get_level() const;
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

