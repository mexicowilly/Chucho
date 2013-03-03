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

namespace chucho
{

/**
 * The configuration of chucho. This namespace is found in the 
 * file <chucho/configuration.hpp>. 
 *  
 * Configuration normally happens automatically when the first
 * @ref logger is requested. However, this is controllable using 
 * the configuratrion style and the @ref allow_default() method.
 * Since, configuration wants to happen using a configuration 
 * file, you may set the file name here. 
 *  
 * @ingroup configuration 
 */
namespace configuration
{

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
     * Configure chucho at the user's discretion. This would be 
     * accomplished by invoking the @ref perform() method. 
     */
    MANUAL,
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
CHUCHO_EXPORT bool allow_default();
/**
 * Whether the environment variable CHUCHO_CONFIG can be used to 
 * retrieve the name of the configuration file. If this is true, 
 * then the first place to look for the name of the 
 * configuration file will be the value of the environment 
 * variable CHUCHO_CONFIG. 
 * 
 * @return whether the environment can be used as a source for 
 *         the name of the configuration file, which is true by
 *         default
 */
CHUCHO_EXPORT bool allow_environment_variable();
/**
 * Return the configuration file name. If the function @ref 
 * set_file_name has not been called, then first the value of 
 * the environment varialbe CHUCHO_CONFIG is checked. If that 
 * value does exist, then it is used as the file name, otherwise 
 * chucho falls back to ./chucho.yaml. 
 * 
 * @return the configuration file name
 */
CHUCHO_EXPORT const std::string& get_file_name();
/**
 * Return the configuration style. AUTOMATIC is the default 
 * style. 
 * 
 * @return the style
 */
CHUCHO_EXPORT style get_style();
/**
 * Perform configuration. This will try and find the 
 * configuration file and load it. The environment variable 
 * CHUCHO_CONFIG will always take precedence in terms of 
 * identifying the configuration file, and if that environment 
 * variable is not set, then the file name used in @ref 
 * set_file_name() will be considered. In the absense of that 
 * file, then ./chucho.yaml will be searched. If that file does 
 * not exist, then, if default configuration is allowed, then it 
 * will be invoked. Otherwise, nothing happens.
 *
 * Calling this function will only actually perform 
 * configuration if the @ref style has been set to MANUAL. 
 * Otherwise, nothing happens. 
 * 
 * @sa @ref set_file_name() 
 * @sa @ref set_allow_default()
 * @sa @ref set_style
 */
CHUCHO_EXPORT void perform();
/**
 * Set whether the default configuration is allowed. In the 
 * absence of a configuration file, chucho can establish a 
 * default configuration. The default configuration conists of a 
 * @ref cout_writer with a basic message format. 
 * 
 * @param allow whether to allow default configuration
 */
CHUCHO_EXPORT void set_allow_default(bool allow);
/**
 * Set whether the environment variable CHUCHO_CONFIG can be 
 * used to retrieve the name of the configuration file. If this 
 * value is true, then the first place to look for the name of 
 * the configuration file will be the value of the environment 
 * variable CHUCHO_CONFIG. 
 * 
 * @param allow whether to allow searching the environment for 
 *              the name of the configuration file
 */
CHUCHO_EXPORT void set_allow_environment_variable(bool allow);
/**
 * Set the configuration file name. The configuration file name 
 * will be used at configuration time to load chucho's 
 * configuration. 
 * 
 * @param name the file name
 */
CHUCHO_EXPORT void set_file_name(const std::string& name);
/**
 * Set the configuration style.
 * 
 * @param stl the style
 */
CHUCHO_EXPORT void set_style(style stl);

}

}

#endif
