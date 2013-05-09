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

#if !defined(CHUCHO_CONFIGURATION_HPP__)
#define CHUCHO_CONFIGURATION_HPP__

#include <chucho/export.hpp>
#include <string>
#include <functional>

namespace chucho
{

/**
 * @class configuration configuration.hpp chucho/configuration.hpp 
 * Configuration normally happens automatically when the first
 * @ref logger is requested. However, this is controllable using 
 * the configuratrion style and the @ref allow_default() method.
 * Since, configuration wants to happen using a configuration 
 * file, you may set the file name here. 
 *  
 * When automatic configuration is allowed (the default), then 
 * the following procedure is undertaken in order to find and 
 * load a valid configuration: 
 *     -# If @ref get_environment_variable is not empty, then
 *      check the environment variable for the name of a valid
 *      configuration file. If
 *      @ref set_environment_variable is
 *      not called, then the default value is CHUCHO_CONFIG. If
 *      the variable's value is the name of a valid
 *      configuration file, then we're done.
 *     -# Check for a configuration file named by @ref
 *      get_file_name. The default file name is ./chucho.yaml.
 *      If @ref get_file_name is empty, then file configuration
 *      is disabled. If the named file is a valid configuration
 *      file, then we're done.
 *     -# If @ref set_fallback has been called, then try to load
 *      the fallback configuration. If this configuration can be
 *      loaded, then we're done.
 *     -# If @ref allow_default is true, then load the default
 *      configuration. The default configuration sets one writer
 *      on the root logger that writes to stdout. Its formatter
 *      uses the pattern <tt>%%d{%%H:%%M:%%S.%%q} %-5p %.36c -
 *      %%m%%n</tt>.
 *  
 * @note The methods in the configuration class are not thread 
 *       safe.
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT configuration
{
public:
    typedef std::function<bool(const std::string&, const std::string&)> unknown_handler_type;

    /**
     * The style of configuration. AUTOMATIC is the default, which 
     * occurs the first time a @ref logger is requested.
     */
    enum class style
    {
        /**
         * Configure chucho the first time a @ref logger is requested.
         */
        AUTOMATIC,
        /**
         * Disable configuration. Using this option would imply that all 
         * chucho loggers would be created and set up using the chucho 
         * API without any configuration file.
         */
        OFF
    };

    /**
     * Whether the default configuration is allowed. In the absence 
     * of a configuration file, chucho can establish a default 
     * configuration. The default configuration conists of a @ref 
     * cout_writer with a basic message format.
     * 
     * @return whether default configuration is allowed, which is 
     *         true by default
     */
    static bool allow_default();
    /**
     * Return the name of the environment variable that is used to 
     * retrieve a configuration file. The default value is 
     * CHUCHO_CONFIG. 
     * 
     * @return the name of the environment variable
     * @sa set_environment_variable 
     */
    static const std::string& get_environment_variable();
    /**
     * Return the fallback configuration that will be used if no 
     * configuration file is found. By default this fallback is 
     * empty. 
     *  
     * @return the fallback configuration 
     * @sa set_fallback
     */
    static const std::string& get_fallback();
    /**
     * Return the configuration file name. If the function @ref 
     * set_file_name has not been called, then the default value of 
     * ./chucho.yaml is returned. 
     * 
     * @return the configuration file name
     */
    static const std::string& get_file_name();
    /**
     * Return the configuration style. AUTOMATIC is the default 
     * style. 
     * 
     * @return the style
     */
    static style get_style();
    static unknown_handler_type get_unknown_handler();
    /**
     * Set whether the default configuration is allowed. In the 
     * absence of a configuration file, chucho can establish a 
     * default configuration. The default configuration conists of a 
     * @ref cout_writer with a basic message format. 
     * 
     * @param allow whether to allow default configuration
     * @sa allow_default 
     */
    static void set_allow_default(bool allow);
    /**
     * Set the environment variable that will be used to find the 
     * configuration file. If this method is not called, then the
     * default variable name is CHUCHO_CONFIG. 
     *  
     * If var is empty, then checking the environment is disabled.
     * 
     * @param var the environment variable that should be used to 
     *            look up the name of the configuration file
     * @sa get_environment_variable 
     */
    static void set_environment_variable(const std::string& var);
    /**
     * Set the fallback configuration. The fallback configuration 
     * will be used if no config file is found. By default there is 
     * no fallback configuration. 
     *  
     * @pre The config text must be valid UTF-8 YAML
     * @param config the fallback configuration
     */
    static void set_fallback(const std::string& config);
    /**
     * Set the configuration file name. The configuration file name 
     * will be used at configuration time to load chucho's 
     * configuration. 
     *  
     * If name is empty, then checking a named file is disabled. 
     * 
     * @param name the file name
     */
    static void set_file_name(const std::string& name);
    /**
     * Set the configuration style.
     * 
     * @param stl the style
     */
    static void set_style(style stl);
    static void set_unknown_handler(unknown_handler_type hndl);

protected:
    friend class logger;

private:
    static void perform();

    static style style_;
    static std::string file_name_;
    static bool allow_default_config_;
    static std::string fallback_;
    static std::string environment_variable_;
    static unknown_handler_type unknown_handler_;
};

inline bool configuration::allow_default()
{
    return allow_default_config_;
}

inline const std::string& configuration::get_environment_variable()
{
    return environment_variable_;
}

inline const std::string& configuration::get_fallback()
{
    return fallback_;
}

inline const std::string& configuration::get_file_name()
{
    return file_name_;
}

inline configuration::style configuration::get_style()
{
    return style_;
}

inline configuration::unknown_handler_type configuration::get_unknown_handler()
{
    return unknown_handler_;
}

inline void configuration::set_allow_default(bool allow)
{
    allow_default_config_ = allow;
}

inline void configuration::set_environment_variable(const std::string& var)
{
    environment_variable_ = var;
}

inline void configuration::set_file_name(const std::string& name)
{
    file_name_ = name;
}

inline void configuration::set_style(style stl)
{
    style_ = stl;
}

inline void configuration::set_unknown_handler(unknown_handler_type hndl)
{
    unknown_handler_ = hndl;
}

}

#endif
