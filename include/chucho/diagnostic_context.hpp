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

#if !defined(CHUCHO_DIAGNOSTIC_CONTEXT_HPP__)
#define CHUCHO_DIAGNOSTIC_CONTEXT_HPP__

#include <chucho/export.h>
#include <string>
#include <map>

namespace chucho
{

/**
 * @class diagnostic_context diagnostic_context.hpp chucho/diagnostic_context.hpp 
 * A diagnostic_context is a thread-specific set of key-value 
 * pairs. The pattern_formatter supports accepting a 
 * diagnostic_context key, the value of which will be inserted 
 * into the message.
 *  
 * @ingroup miscellaneous 
 */
class CHUCHO_EXPORT diagnostic_context
{
public:
    /**
     * Return a reference to the value of a given key. If the key 
     * does not exist in this context, then it will be added. 
     * 
     * @param key the key
     * @return a reference to the key's value
     */
    static std::string& at(const std::string& key);
    /**
     * Clear all key-value pairs.
     */
    static void clear();
    /**
     * Does this context have any values?
     * 
     * @return true if the context has no values
     */
    static bool empty();
    /**
     * Erase a given key and its value from the context.
     * 
     * @param key the key to erase
     */
    static void erase(const std::string& key);
    /**
     * Get a copy of all key-value pairs.
     * 
     * @return a copy of this context
     */
    static std::map<std::string, std::string> get();
    /**
     * Assign a given collection of key-value pairs to this context. 
     * Any keys in this context prior to calling this method are 
     * erased. 
     * 
     * @param ctx the context to set
     */
    static void set(const std::map<std::string, std::string>& ctx);

private:
    static std::map<std::string, std::string>& get_map();
};

}

#endif
