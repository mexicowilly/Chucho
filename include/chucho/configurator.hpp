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

#if !defined(CHUCHO_CONFIGURATOR_HPP__)
#define CHUCHO_CONFIGURATOR_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/status_reporter.hpp>
#include <chucho/security_policy.hpp>
#include <map>
#include <string>

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
                                         std::unique_ptr<configurable_factory>&& fact);

    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Make a configurator.
     */
    configurator(security_policy& sec_pol);
	/**
	 * Destroy a configurator.
	 */
	virtual ~configurator();
    //@}

    /**
     * Perform configuration. This involves reading the 
     * configuration file, parsing its contents, and creating the 
     * objects described therein. 
     * 
     * @param in the input stream
     */
    virtual void configure(std::istream& in) = 0;
    /**
     * Return the @ref security_policy associated with this 
     * configurator. 
     * 
     * @return the security policy
     */
    security_policy& get_security_policy();

protected:
    /**
     * The configuration class must be able to call some protected 
     * and private members of this class. 
     */
    friend class configuration;

    /**
     * Return all known factories. Subclasses will call this method 
     * while performing configuration. 
     * 
     * @return the factories
     */
    static std::map<std::string, std::unique_ptr<configurable_factory>>& get_factories();

    /**
     * Add variables. As the configurator is operating, it may 
     * encounter variables. This method is used to store them. 
     * 
     * @param vars the variables
     */
    void add_variables(const std::map<std::string, std::string>& vars);
    /**
     * Resolve variables. As the configurator encounters pieces of 
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
     * This is invoked by configuration::perform() and 
     * configuration::set_configuration(). 
     */
    CHUCHO_NO_EXPORT static void initialize();
    CHUCHO_NO_EXPORT static void initialize_impl();

    std::map<std::string, std::string> variables_;
    security_policy& security_policy_;
};

inline void configurator::add_variables(const std::map<std::string, std::string>& vars)
{
    variables_.insert(vars.begin(), vars.end());
}

inline security_policy& configurator::get_security_policy()
{
    return security_policy_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
