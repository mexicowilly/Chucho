#if !defined(CHUCHO_MEMENTO_HPP__)
#define CHUCHO_MEMENTO_HPP__

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
     * Create a memento for the given @ref configurator. A memento 
     * will usually register its handlers at creation time. A 
     * handler is registered for each key that a memento expects to 
     * receive, and when the handler is invoked, the key's value is 
     * processed. 
     * 
     * @param cfg the configurator that is performing configuration
     */
    memento(const configurator& cfg);

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
    virtual void handle(std::shared_ptr<configurable> cnf);

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
     * Set a handler. The handler will be invoked when the @ref 
     * configurator encounters the given key. 
     * 
     * @param key the key of the handler
     * @param hand the function
     */
    void set_handler(const std::string& key, handler hand);
    /**
     * Convert a piece of text to lower case. This is used for 
     * case-insensitive comparison.
     * 
     * @param value the text to case fold 
     * @return the lowercase version of the text
     */
    std::string to_lower(const std::string& value);

    /**
     * The @ref configurator that is perform configuration.
     */
    const configurator& cfg_;

private:
    std::map<std::string, handler> handlers_;
};

inline void memento::set_handler(const std::string& key, handler hand)
{
    handlers_[key] = hand;
}

}

#endif
