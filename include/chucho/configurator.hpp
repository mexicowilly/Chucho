#if !defined(CHUCHO_CONFIGURATOR_HPP__)
#define CHUCHO_CONFIGURATOR_HPP__

#include <chucho/status_reporter.hpp>
#include <chucho/configuration.hpp>
#include <map>

namespace chucho
{

class configurable_factory;

/** 
 * @class configurator configurator.hpp chucho/configurator.hpp
 * Configure chucho. A configurator is used to configure chucho
 * using an input stream. Configuration by default happens 
 * automatically when the first @ref logger is requested. A 
 * configurator is created that can perform its duties with the 
 * information at hand about the configuration file, or lack 
 * thereof. 
 *  
 * @ingroup configuration
 * @sa @ref configuration 
 */
class CHUCHO_EXPORT configurator : public status_reporter
{
public:
    /**
     * Add a factory. Any class that expects to be instantiated 
     * during configuration time must register itself here with its 
     * factory. All predefined chucho configurable classes 
     * automatically do this. You only need to call this method if 
     * you have a non-chucho-defined type that you would like to be 
     * @ref configurable. 
     * 
     * @param name the name as it will appear in the configuration 
     *             file
     * @param fact the factory that will be used to manage creation 
     *             of the class in question
     */
    static void add_configurable_factory(const std::string& name,
                                         std::shared_ptr<configurable_factory> fact);

    /**
     * @name Constructor
     */
    //@{
    /**
     * Make a configurator.
     */
    configurator();
    //@}

    /**
     * Perform configuration. This involves reading the 
     * configuration file, parsing its contents, and creating the 
     * objects described therein. 
     * 
     * @param in the input stream
     */
    virtual void configure(std::istream& in) = 0;

protected:
    /**
     * The @ref configuration::perform() method must be able to call 
     * some protected and private members of this class. 
     */
    friend CHUCHO_EXPORT void configuration::perform();

    /**
     * Return all known factories. Subclasses will call this method 
     * while performing configuration. 
     * 
     * @return the factories
     */
    static std::map<std::string, std::shared_ptr<configurable_factory>>& get_factories();

    /**
     * Add variables. As the configurator is operating, it may 
     * encounter variables. This method is used to store them. 
     * 
     * @param vars the variables
     */
    void add_variables(const std::map<std::string, std::string>& vars);
    /**
     * Resove variables. As the configurator encounters pieces of 
     * text, they may contain variable references. These are 
     * resolved here. 
     *
     * @param val the string containing possible variable references
     * 
     * @return the resolved piece of text
     */
    std::string resolve_variables(const std::string& val);

private:
    /**
     * This is invoked by configuration::perform().
     */
    static void initialize();

    std::map<std::string, std::string> variables_;
};

inline void configurator::add_configurable_factory(const std::string& name,
                                                   std::shared_ptr<configurable_factory> fact)
{
    get_factories()[name] = fact;
}

inline void configurator::add_variables(const std::map<std::string, std::string>& vars)
{
    variables_.insert(vars.begin(), vars.end());
}

}

#endif
