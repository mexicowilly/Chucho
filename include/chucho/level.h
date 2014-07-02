/*
 * Copyright 2013-2014 Will Mason
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

#if !defined(CHUCHO_LEVEL_H__)
#define CHUCHO_LEVEL_H__

/**
 * @file 
 * Levels of Chucho logging. 
 * @copydoc chucho::level
 *  
 * @ingroup c_levels 
 */

#include <chucho/syslog_constants.h>
#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque structure that you don't care about.
 */
typedef struct chucho_level chucho_level;

/**
 * Add a custom level. This allows the @ref chucho_get_level 
 * function to return a custom level. This is primarily used for
 * allowing custom levels to appear in configuration files. If 
 * you call this method before configuration happens, then your 
 * custom level will be able to be resolved by the configurator.
 * 
 * @param[out] lvl the level to add
 * @param[in] name the name of the level 
 * @param[in] value the value 
 * @param[in] sev the syslog severity
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_add_level(const chucho_level** lvl,
                                         const char* const name,
                                         int value,
                                         chucho_syslog_severity sev);

/**
 * Return a named level.
 * 
 * @param[out] lvl the level corresponding to the name
 * @param[in] name the name of the level
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_get_level(const chucho_level** lvl, const char* const name);
/**
 * @name Built-in Levels
 * @{
 */
/**
 * Return the trace level. Trace has a level of 0.
 * 
 * @return the trace level
 */
CHUCHO_EXPORT const chucho_level* chucho_trace_level(void);
/**
 * Return the debug level. Debug has a level of 10000.
 * 
 * @return the debug level
 */
CHUCHO_EXPORT const chucho_level* chucho_debug_level(void);
/**
 * Return the info level. Info has a level of 20000.
 * 
 * @return the info level
 */
CHUCHO_EXPORT const chucho_level* chucho_info_level(void);
/**
 * Return the warn level. Warn has a level of 30000.
 * 
 * @return the warn level
 */
CHUCHO_EXPORT const chucho_level* chucho_warn_level(void);
/**
 * Return the error level. Error has a level of 40000.
 * 
 * @return the error level
 */
CHUCHO_EXPORT const chucho_level* chucho_error_level(void);
/**
 * Return the fatal level. Fatal has a level of 50000.
 * 
 * @return the fatal level
 */
CHUCHO_EXPORT const chucho_level* chucho_fatal_level(void);
/**
 * Return the off level. Off has a level that is none of your 
 * business. 
 * 
 * @return the off level
 */
CHUCHO_EXPORT const chucho_level* chucho_off_level(void);
/** @} */
/**
 * Return the name of a level.
 * 
 * @param[in] lvl the level
 * @param[out] name the name
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lvl_get_name(const chucho_level* lvl, const char** name);
/**
 * Return the syslog severity of a level.
 * 
 * @param[in] lvl the level
 * @param[out] sev the syslog severity
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lvl_get_syslog_severity(const chucho_level* lvl, chucho_syslog_severity* sev);
/**
 * Return the value of a level.
 * 
 * @param[in] lvl the level
 * @param[out] val the value
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lvl_get_value(const chucho_level* lvl, int* val);

#if defined(__cplusplus)
}
#endif

#endif
