/*
 * Copyright 2013 Will Mason
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

#if !defined(CHUCHO_FORMATTER_HPP__)
#define CHUCHO_FORMATTER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>
#include <string>

namespace chucho
{

/**
 * @defgroup formatters 
 * Formatters turn a log @ref event into a piece of text so that
 * a writer can write the event. 
 */

/**
 * @class formatter formatter.hpp chucho/formatter.hpp
 * Format an event. A formatter is responsible for turning a log 
 * event into a piece of text. Each @ref writer will have one 
 * formatter associated with it. 
 *  
 * @ingroup formatters 
 */
class CHUCHO_EXPORT formatter : public status_reporter,
                                public configurable
{
public:
    /**
     * Format the event. The given event is turned into a piece of 
     * text. The encoding of the resulting piece of text is 
     * undefined. The encoding will be a mash-up of the default 
     * encoding used by the system, any other encodings used in the 
     * event's message, and the encoding of the compiler when 
     * creating the constants for file name, line number and 
     * function name. 
     * 
     * @param evt the event to format
     * @return the piece of text
     */
    virtual std::string format(const event& evt) = 0;
};

}

#endif
