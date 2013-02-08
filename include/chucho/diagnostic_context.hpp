#if !defined(CHUCHO_DIAGNOSTIC_CONTEXT_HPP__)
#define CHUCHO_DIAGNOSTIC_CONTEXT_HPP__

#include <chucho/export.hpp>
#include <string>
#include <map>

namespace chucho
{

/**
 * @class diagnostic_context diagnostic_context.hpp chucho/diagnostic_context.hpp 
 * A diagnostic_context is a thread-specific set of key-value 
 * pairs. The pattern_formatter supports accepting a 
 * diagnostic_context key, which will be inserted into the 
 * message as the key's value. 
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
};

}

#endif
