/*
 * Copyright 2013-2017 Will Mason
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

#if !defined(CHUCHO_CONFIGURATION_H__)
#define CHUCHO_CONFIGURATION_H__

/**
 * @file 
 * Functions for controlling the configuration of Chucho. 
 * @copydoc chucho::configuration
 *  
 * @ingroup c_configuration 
 */

#include <chucho/security_policy.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @copydoc chucho::configuration::style
 */
typedef enum
{
    /**
     * Configure chucho the first time a @ref chucho::logger is 
     * requested. 
     */
    CHUCHO_CONFIGURATION_STYLE_AUTOMATIC,
    /**
     * Disable configuration. Using this option would imply that all 
     * chucho loggers would be created and set up using the chucho 
     * API without any configuration file.
     */
    CHUCHO_CONFIGURATION_STYLE_OFF
} chucho_configuration_style;

/**
 * @copydoc chucho::configuration::unknown_handler_type
 * @return a non-zero value if the handler was successful
 */
typedef void (*chucho_unknown_handler_type)(const char* const key, const char* const val);
/**
 * Whether the default configuration is allowed. In the absence 
 * of a configuration file, chucho can establish a default 
 * configuration. The default configuration conists of a @ref 
 * chucho::cout_writer with a basic message format.
 *  
 * @param[out] val whether default configuration is allowed 
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_allow_default
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_allow_default(int* val);
/**
 * Return the name of the environment variable that is used to 
 * retrieve a configuration file. The default value is 
 * CHUCHO_CONFIG. 
 * 
 * @param[out] var the environment variable
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_environment_variable 
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_environment_variable(const char** var);
/**
 * Return the fallback configuration that will be used if no 
 * configuration file is found. By default this fallback is 
 * empty. 
 *  
 * @param[out] fb the fallback configuration
 * @sa chucho_cnf_set_fallback
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_fallback(const char** fb);
/**
 * Return the configuration file name. If the function @ref 
 * chucho_cnf_set_file_name has not been called, then the 
 * default value of ./chucho.yaml is returned. 
 * 
 * @param[out] name the file name
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_file_name
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_file_name(const char** name);
/**
 * Return the name of the last file that was successfully loaded 
 * for Chucho configuration. 
 * 
 * @param[out] name the loaded file name or NULL if no 
 *       configuration file has been loaded
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_loaded_file_name(const char** name);
/**
 * Return the maximum size of a configuration file. Files bigger 
 * than this value will not be loaded. The default maximum is 
 * 100k. 
 * 
 * @param[out] sz the maximum file size
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_max_size
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_max_size(size_t* sz);
/**
 * Return the security policy. The security policy is used to 
 * control the range of user inputs that can be found in a 
 * configuration. 
 *  
 * @post Ownership of the pol parameter is returned to the 
 *       caller, and must be released with @ref
 *       chucho_release_security_policy.
 * 
 * @param pol the security policy
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa security_policy.h
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_security_policy(chucho_security_policy** pol);
/**
 * Return the configuration style. 
 * CHUCHO_CONFIGURATION_STYLE_AUTOMATIC is the default style. 
 * 
 * @param[out] sty the style
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_style
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_style(chucho_configuration_style* sty);
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
 * @param[out] func the function to handle unknown elements
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_set_unknown_handler
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_get_unknown_handler(chucho_unknown_handler_type* func);
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
 *      first @ref chucho::logger.
 * @note This configuration does not imply the chain of 
 *       fallbacks that occur in initial configuration. In this
 *       case only the last file read or the file named by
 *       get_file_name() is tried.
 * @param[out] rc non-zero if the configuration was performed 
 *         successfully, zero otherwise
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_reconfigure(int* rc);
/**
 * Set whether the default configuration is allowed. In the 
 * absence of a configuration file, chucho can establish a 
 * default configuration. The default configuration conists of a 
 * @ref chucho::cout_writer with a basic message format. 
 * 
 * @param[in] val whether to allow default configuration
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_allow_default 
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_allow_default(int val);
/**
 * Set the environment variable that will be used to find the 
 * configuration file. If this method is not called, then the
 * default variable name is CHUCHO_CONFIG. 
 *  
 * If var is NULL, then checking the environment is disabled.
 * 
 * @param[in] var the environment variable that should be used 
 *            to look up the name of the configuration file
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_get_environment_variable 
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_environment_variable(const char* const var);
/**
 * Set the fallback configuration. The fallback configuration 
 * will be used if no config file is found. By default there is 
 * no fallback configuration. 
 *  
 * If fb is NULL, then there is no fallback configuration. 
 *  
 * @param[in] fb the fallback configuration
 * @return a value from @ref return_code.h indicating success or
 *         failure
 * @sa chucho_cnf_get_fallback
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_fallback(const char* const fb);
/**
 * Set the configuration file name. The configuration file name 
 * will be used at configuration time to load chucho's 
 * configuration. 
 *  
 * If name is NULL, then checking a named file is disabled. 
 * 
 * @param[in] name the file name
 * @sa chucho_cnf_get_file_name
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_file_name(const char* const name);
/**
 * Set the maximum size for a configuration file. If any file 
 * exceeds this size, it will be rejected. By default this 
 * value is 100k. 
 * 
 * @param[in] sz the maximum size
 * @sa chucho_cnf_get_max_size
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_max_size(size_t sz);
/**
 * Set the configuration style.
 * 
 * @param[in] sty the style
 * @sa chucho_cnf_get_style
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_style(chucho_configuration_style sty);
/**
 * Set the unknown element handler. The unknown element 
 * handler is a callback that the application may set to process 
 * elements in the configuration that Chucho itself does not 
 * understand. This allows the application to use the Chucho 
 * configuration for its own configurable elements. 
 *  
 * The unknown handler takes two arguments, both of which are 
 * <tt>const char* const</tt>. The first is the key and the 
 * second is the value. The unknown handler will only be called 
 * if what was found in the configuration can be converted to a 
 * key-value pair. 
 *  
 * @note The unknown element handler is only supported in YAML 
 *       configurations.
 * 
 * @param[in] func the unknown element handler
 * @sa chucho_cnf_get_unknown_handler
 */
CHUCHO_EXPORT chucho_rc chucho_cnf_set_unknown_handler(chucho_unknown_handler_type func);

#if defined(__cplusplus)
}
#endif

#endif
