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
#include <chucho/status_reporter.hpp>
#include <string>
#include <functional>

namespace chucho
{

class logger;

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
 * When an unknown element is encountered during configuration 
 * processing, and if the element can be understood as a 
 * key-value pair, then a callback may be used for the 
 * application to process the element. This allows Chucho and 
 * application configuration data to appear in the same file 
 * side by side.
 *  
 * @note The methods in the configuration class are not thread 
 *       safe.
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT configuration
{
public:
    /**
     * The signature required for a function to handle unknown 
     * configuration key-value pairs. The first parameter to the 
     * function is the key and the second is the value. 
     */
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
     * Return the name of the last file that was successfully loaded 
     * for Chucho configuration. 
     * 
     * @return the loaded file name
     */
    static const std::string& get_loaded_file_name();
    static std::size_t get_max_size();
    /**
     * Return the configuration style. AUTOMATIC is the default 
     * style. 
     * 
     * @return the style
     */
    static style get_style();
    /**
     * Return the unknown element handler. The unknown element 
     * handler is a callback that the application may set to process 
     * elements in the configuration that Chucho itself does not 
     * understand. This allows the application to use the Chucho 
     * configuration for its own configurable elements. 
     *  
     * @note The unknown element handler is only supported in YAML 
     *       configurations.
     * 
     * @return the function to handle unknown elements
     */
    static unknown_handler_type get_unknown_handler();
    /**
     * Reconfigure Chucho by reading the last configuration file 
     * again. The file read may either be the last file that was 
     * successfully processed by Chucho, or the file named by 
     * get_file_name(), if there is no record of a successful 
     * configuration file. This allows a file that didn't exist 
     * initally to be retried. 
     *  
     * The purpose of this method is to allow for the common use 
     * case of wanting to update the running configuration after the
     * configuration file has been updated. This occurs commonly in 
     * servers, which re-read their configuration files upon 
     * receiving SIGHUP.
     *  
     * If the reconfiguration fails for any reason, then the current 
     * running configuration of Chucho is not affected. 
     *  
     * @pre Chucho must have already undergone its initial 
     *      configuration, which is initiated by requesting the
     *      first @ref logger.
     * @note This configuration does not imply the chain of 
     *       fallbacks that occur in initial configuration. In this
     *       case only the last file read or the file named by
     *       get_file_name() is tried.
     * @return true if the configuration was performed successfully,
     *         false otherwise
     */
    static bool reconfigure();
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
     * @throw exception if the UTF-8 is not valid
     * @sa get_fallback
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
    static void set_max_size(std::size_t mx);
    /**
     * Set the configuration style.
     * 
     * @param stl the style
     */
    static void set_style(style stl);
    /**
     * Set the unknown element handler. The unknown element 
     * handler is a callback that the application may set to process 
     * elements in the configuration that Chucho itself does not 
     * understand. This allows the application to use the Chucho 
     * configuration for its own configurable elements. 
     *  
     * The unknown handler takes two arguments, both of which are 
     * <tt>const std::string&</tt>. The first is the key and the 
     * second is the value. The unknown handler will only be called 
     * if what was found in the configuration can be converted to a 
     * key-value pair. 
     *  
     * @note The unknown element handler is only supported in YAML 
     *       configurations.
     * 
     * @param hndl the unknown element handler
     */
    static void set_unknown_handler(unknown_handler_type hndl);

protected:
    friend class logger;

private:
    class reporter : public chucho::status_reporter
    {
    public:
        reporter();

        void error(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
        void info(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
        void warning(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const;
    };

    CHUCHO_NO_EXPORT static bool configure_from_file(const std::string& file_name, reporter& report);
    CHUCHO_NO_EXPORT static void perform(std::shared_ptr<logger> root_logger);
};

inline configuration::reporter::reporter()
{
    set_status_origin("configuration");
}

inline void configuration::reporter::error(const std::string& message, std::exception_ptr ex) const
{
    report_error(message, ex);
}

inline void configuration::reporter::info(const std::string& message, std::exception_ptr ex) const
{
    report_info(message, ex);
}

inline void configuration::reporter::warning(const std::string& message, std::exception_ptr ex) const
{
    report_warning(message, ex);
}

}

#endif
