/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_ON_START_FILE_ROLL_TRIGGER_HPP_)
#define CHUCHO_ON_START_FILE_ROLL_TRIGGER_HPP_

#include <chucho/file_roll_trigger.hpp>

namespace chucho
{

/**
 * @class on_start_file_roll_trigger on_start_file_roll_trigger.hpp chucho/on_start_file_roll_trigger.hpp
 * A @ref file_roll_trigger that only triggers for the first
 * log event. The allows test environments with busy log files
 * to present one log file per run.
 *
 * @ingroup rolling
 */
class CHUCHO_EXPORT on_start_file_roll_trigger : public file_roll_trigger
{
public:
    /**
     * If this trigger has never fired before, then it fires.
     *
     * @param active_file the currently active file name
     * @param e the log event
     * @return bool true if this trigger has never been visited before
     */
    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    bool has_fired_{false};
};

}

#endif
