/*
 * Copyright 2013-2019 Will Mason
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

#if !defined(CHUCHO_BUILD) && defined(__cplusplus)
#error "When using C++, you should include configuration.hpp"
#endif

#include <chucho/export.h>
#include <stdbool.h>

/**
 * @file 
 * Functions for controlling the configuration of Chucho. 
 * @copydoc chucho::configuration
 *  
 * @ingroup c_configuration 
 */

#if defined(__cplusplus)
extern "C"
{
#endif


/**
 * Whether the default configuration is allowed. In the absence 
 * of a configuration file, chucho can establish a default 
 * configuration. The default configuration conists of a @ref 
 * chucho::cout_writer with a basic message format.
 *  
 * @return whether default configuration is allowed
 * @sa chucho_cnf_set_allow_default
 */
CHUCHO_EXPORT bool chucho_cnf_allow_default(void);
/**
 * Return the name of the environment variable that is used to 
 * retrieve a configuration file. The default value is 
 * CHUCHO_CONFIG. 
 * 
 * @return the environment variable
 * @sa chucho_cnf_set_environment_variable
 */
CHUCHO_EXPORT const char* chucho_cnf_get_environment_variable(void);
/**
 * Return the fallback configuration that will be used if no 
 * configuration file is found. By default this fallback is 
 * empty. 
 *  
 * @return the fallback configuration
 * @sa chucho_cnf_set_fallback
 */
CHUCHO_EXPORT const char* chucho_cnf_get_fallback(void);
/**
 * Return the configuration file name. If the function @ref 
 * chucho_cnf_set_file_name has not been called, then the 
 * default value of ./chucho.yaml is returned. 
 * 
 * @return the file name
 * @sa chucho_cnf_set_file_name
 */
CHUCHO_EXPORT const char* chucho_cnf_get_file_name(void);
/**
 * Set whether the default configuration is allowed. In the 
 * absence of a configuration file, chucho can establish a 
 * default configuration. The default configuration conists of a 
 * @ref chucho::cout_writer with a basic message format. 
 * 
 * @param[in] val whether to allow default configuration
 * @sa chucho_cnf_allow_default
 */
CHUCHO_EXPORT void chucho_cnf_set_allow_default(bool val);
/**
 * Set the environment variable that will be used to find the 
 * configuration file. If this method is not called, then the
 * default variable name is CHUCHO_CONFIG. 
 *  
 * If var is NULL, then checking the environment is disabled.
 * 
 * @param[in] var the environment variable that should be used 
 *            to look up the name of the configuration file
 * @sa chucho_cnf_get_environment_variable
 */
CHUCHO_EXPORT void chucho_cnf_set_environment_variable(const char* const var);
/**
 * Set the fallback configuration. The fallback configuration 
 * will be used if no config file is found. By default there is 
 * no fallback configuration. 
 *  
 * If fb is NULL, then there is no fallback configuration. 
 *  
 * @param[in] fb the fallback configuration
 * @sa chucho_cnf_get_fallback
 */
CHUCHO_EXPORT void chucho_cnf_set_fallback(const char* const fb);
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
CHUCHO_EXPORT void chucho_cnf_set_file_name(const char* const name);

#if defined(__cplusplus)
}
#endif

#endif
