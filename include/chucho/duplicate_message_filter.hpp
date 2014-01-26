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

#if !defined(CHUCHO_DUPLICATE_MESSAGE_FILTER_HPP__)
#define CHUCHO_DUPLICATE_MESSAGE_FILTER_HPP__

#include <chucho/filter.hpp>

namespace chucho
{

/**
 * @class duplicate_message_filter duplicate_message_filter.hpp chucho/duplicate_message_filter.hpp 
 * A filter for removing duplicate messages. The message of an 
 * event is examined to see if it matches the last message 
 * written. If it does, then the event is denied. Once a new 
 * message is received, then the filter reports the number of 
 * duplicate messages seen to the @ref status_manager. 
 *  
 * @ingroup filters 
 */
class CHUCHO_EXPORT duplicate_message_filter : public filter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a duplicate_message_filter.
     */
    duplicate_message_filter();
    //@}

    /**
     * Return DENY if the event's message matches the last message 
     * seen. Otherwise return NEUTRAL. 
     * 
     * @param evt the event to evaluate
     * @return the result
     */
    result evaluate(const event& evt) override;

private:
    std::string message_;
    std::size_t count_;
};

}

#endif
