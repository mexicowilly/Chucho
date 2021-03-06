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

#if !defined(CHUCHO_DUPLICATE_MESSAGE_FILTER_HPP_)
#define CHUCHO_DUPLICATE_MESSAGE_FILTER_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writeable_filter.hpp>

namespace chucho
{

/**
 * @class duplicate_message_filter duplicate_message_filter.hpp chucho/duplicate_message_filter.hpp 
 * A filter for removing duplicate messages. The message of an 
 * event is examined to see if it matches the last message 
 * written. If it does, then the event is denied. Once a new 
 * message is received, then the filter writes that a message
 * was repeated a certain number of times.
 *  
 * @ingroup filters 
 */
class CHUCHO_EXPORT duplicate_message_filter : public writeable_filter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a duplicate_message_filter. This filter will not have a
     * writer set yet.
     *
     * @param name the name of the filter
     */
    duplicate_message_filter(const std::string& name);
    /**
     * Construct a duplicate_message_filter.
     *
     * @param name the name of the filter
     * @param wrt the writer
     */
    duplicate_message_filter(const std::string& name, writer& wrt);
    //@}

    /**
     * Return DENY if the event's message matches the last message 
     * seen. Otherwise return NEUTRAL. 
     * 
     * @param evt the event to evaluate
     * @return the result
     */
    virtual result evaluate(const event& evt) override;

private:
    std::string message_;
    std::size_t count_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
