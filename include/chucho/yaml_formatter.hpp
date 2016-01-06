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

#if !defined(CHUCHO_YAML_FORMATTER_HPP__)
#define CHUCHO_YAML_FORMATTER_HPP__

#include <chucho/formatter.hpp>

namespace chucho
{

/**
 * @class yaml_formatter yaml_formatter.hpp chucho/yaml_formatter.hpp 
 * A formatter that outputs the event to a <a 
 * href="http://yaml.org/spec/1.2/spec.html">YAML</a> mapping. 
 * The mapping is a sequence entry named "event" followed by the 
 * following mapped keys: version, milliseconds_since_epoch, 
 * file_name, line_number, function_name, logger, level, marker,
 * thread identifier, and message. If the event has no marker,
 * then the YAML won't have that key, either.
 *  
 * The only mapped key that does not correspond to a field in an 
 * event is version. This is used to communicate the version of 
 * the event-to-YAML formatting that is represented by the YAML. 
 *  
 * A typical entry might look like: 
 * @verbatim 
 - event: 
     version: 1
     milliseconds_since_epoch: 1369524662227
     file_name: "/Users/will/Projects/chucho/test/remote_writer_test.cpp"
     line_number: 119
     function_name: "int main(int, char **)"
     logger: "client.1"
     level: "INFO"
     thread: "09347589"
     message: |
        hello
@endverbatim
 *  
 * If the message is not encoded with UTF-8, then any invalid 
 * byte sequences will be escaped out, in order to create valid 
 * UTF-8. For example, if a byte 0xff apears in the message, 
 * then it will be replaced with the text "\xff". In other 
 * words, invalid UTF-8 is escaped into a backslash followed by
 * a textual representation of the hexadeimal value of the byte.
 *  
 * This formatter is used by the remote_writer to serialize an 
 * event for transport over the wire. 
 * 
 * @ingroup formatters
 */
class CHUCHO_EXPORT yaml_formatter : public formatter
{
public:
    /**
     * The version of this formatter, which is 1.
     */
    static const unsigned VERSION;

    /**
     * Format the event. A YAML sequence entry is created with 
     * mappings for the keys, version, milliseconds_since_epoch, 
     * file_name, line_number, function_name, logger, level, marker, 
     * and message. 
     * 
     * @note Please see the class documentation for details.

     * @param evt the event to format
     * @return the YAML
     */
    virtual std::string format(const event& evt) override;
};

}

#endif
