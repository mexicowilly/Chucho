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

#if !defined(CHUCHO_MEMENTO_HPP__)
#define CHUCHO_MEMENTO_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/configurator.hpp>
#include <chucho/configurable.hpp>
#include <functional>

namespace chucho
{

/**
 * @class memento memento.hpp chucho/memento.hpp
 * An abstract representation of an object that can be used to 
 * create a real one. Mementos are used by the configuration 
 * system to create @ref configurable objects. You only need to 
 * worry about mementos if you create custom types that you 
 * would like to make @ref configurable. As a configuration file 
 * is read by the @ref configurator, the key-value pairs that it 
 * encounters in the file are fed to a collection of mementos 
 * that gather information about objects that will be created. 
 * Once the memento's realization is complete during 
 * configuration, a @ref configurable_factory is used to create 
 * a real object using its memento. 
 *
 * @ingroup configuration
 */
class CHUCHO_EXPORT memento : public status_reporter
{
public:
	/**
	 * @name Constructor and Destructor
	 * @{
	 */
    /**
     * Create a memento for the given @ref configurator. A memento 
     * will usually register its handlers at creation time. A 
     * handler is registered for each key that a memento expects to 
     * receive, and when the handler is invoked, the key's value is 
     * processed. 
     * 
     * @param cfg the configurator that is performing configuration
     */
    memento(configurator& cfg);
    /**
     * Destroy the memento.
     */
    virtual ~memento();
    /** @} */

    /**
     * Handle a key-value pair from the configuration file. A 
     * memento will register handlers at creation time. When the 
     * @ref configurator encounters a key for a particular memento, 
     * asks the memento to handle the value associated with that 
     * key. The memento will invoke the appropriate handler. 
     * 
     * @param key the key
     * @param value the value
     */
    void handle(const std::string& key, const std::string& value);
    /**
     * Handle a @ref configurable. Configurables can be nested. When 
     * a child @ref configurable is created, it is passed to the 
     * parent memento for processing. Usually, the parent memento 
     * will hang onto this @ref configurable so that the @ref 
     * configurable_factory can pass it to a constructor. 
     * 
     * @param cnf the configurable
     */
    virtual void handle(std::unique_ptr<configurable>&& cnf);

protected:
    /**
     * The handler type.
     */
    typedef std::function<void(const std::string&)> handler;

    /**
     * Resolve a boolean from a piece of text. If the text is 
     * anything other than "0" or "false" (using a case-insensitive 
     * comparison), then it is considered true. 
     * 
     * @param value the piece of text
     * @return true if the value is anything other than "0" or 
     *         "false"
     */
    bool boolean_value(const std::string& value);
    /**
     * The handler that should be used when no specific handler is given
     * for a key. The base method simply reports an error.
     *
     * @param key the key
     * @param value the value
     */
    virtual void default_handler(const std::string& key, const std::string& value);
    /**
     * Set an alias for a given key. For example, if you have a 
     * handler for the key "hello", then you may want to set an 
     * alias of "hi", which will just invoke the "hello" handler. 
     * 
     * @param key the key for which to create an alias
     * @param alias the alias for key
     */
    void set_alias(const std::string& key, const std::string& alias);
    /**
     * Set a handler. The handler will be invoked when the @ref 
     * configurator encounters the given key. 
     * 
     * @param key the key of the handler
     * @param hand the function
     */
    void set_handler(const std::string& key, handler hand);
    /**
     * Validate a piece of text found in a configuration with the 
     * current security policy. 
     * 
     * @param key the security policy key
     * @param val the value to validate
     * @return the value if the validation passes
     */
    std::string validate(const std::string& key, const std::string& val) const;
    /**
     * Validate an integer found in a configuration with the current
     * security policy. 
     * 
     * @param key the security policy key
     * @param val the value to validate
     * @return the value if the validation passes
     */
    template <typename val_type>
    val_type validate(const std::string& key, const val_type& val) const;

    /**
     * The @ref configurator that is perform configuration.
     */
    configurator& cfg_;

private:
    std::map<std::string, handler> handlers_;
    std::multimap<std::string, std::string> unconnected_aliases_;
};

inline std::string memento::validate(const std::string& key, const std::string& val) const
{
    return cfg_.get_security_policy().validate(key, val);
}

template <typename val_type>
val_type memento::validate(const std::string& key, const val_type& val) const
{
    return static_cast<val_type>(cfg_.get_security_policy().validate(key, val));
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
