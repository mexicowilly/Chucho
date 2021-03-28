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

#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_HPP_)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_HPP_

#include <chucho/file_roll_trigger.hpp>
#include <cstdint>

namespace chucho
{

/**
 * @class size_file_roll_trigger size_file_roll_trigger.hpp chucho/size_file_roll_trigger.hpp
 * A @ref file_roll_trigger that monitors file size. When the 
 * active log file reaches this trigger's maximum size, then it 
 * will signal that now would be a good time to roll the file. 
 *  
 * @ingroup rolling 
 */
class CHUCHO_EXPORT size_file_roll_trigger : public file_roll_trigger
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a size_file_roll_trigger.
     * 
     * @param max_size the maximum size that a file can reach before 
     *                 rolling
     */
    size_file_roll_trigger(std::uintmax_t max_size);
    //@}

    /**
     * Return this trigger's maximum size.
     * 
     * @return this trigger's maximum size
     */
    std::uintmax_t get_max_size() const;
    /**
     * If the size of active_file is greater than this trigger's 
     * maximum size, then this trigger fires. 
     * 
     * @param active_file the currently active file name
     * @param e the log event
     * @return bool true if the file size has exceeded the limit
     */
    virtual bool is_triggered(const std::string& active_file, const event& e) override;

private:
    std::uintmax_t max_size_;
};

inline std::uintmax_t size_file_roll_trigger::get_max_size() const
{
    return max_size_;
}

}

#endif
