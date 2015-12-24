/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_LOGGER_H__)
#define CHUCHO_LOGGER_H__

/**
 * @file 
 * @copydoc chucho::logger 
 *  
 * @sa log.h for C API macros used in log event filtering
 *  
 * @ingroup c_loggers 
 */

#include <chucho/level.h>
#include <chucho/writer.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_logger chucho_logger;

/**
 * @name Creation and Release
 * @{
 */
/**
 * Create a logger with the given name.
 * 
 * @post Ownership of the logger is transferred to the caller, 
 *       and it must be released with the @ref
 *       chucho_release_logger() function.
 * 
 * @param[out] lgr the logger to create
 * @param[in] name the name of the logger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_logger(chucho_logger** lgr, const char* const name);
/**
 * Release a logger.
 * 
 * @post Ownership of the lgr parameter is transferred to the 
 *       callee.
 * 
 * @param[in] lgr the logger to release
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_logger(chucho_logger* lgr);
/** @}  */
/**
 * @copydoc chucho::logger::remove_unused_loggers
 *
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_remove_unused_loggers(void);
/**
 * Add a writer to a logger.
 *  
 * @post Ownership of the wrt parameter is transferred to the 
 *       callee.
 * 
 * @param[in] lgr the logger
 * @param[in] wrt the writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_add_writer(chucho_logger* lgr, chucho_writer* wrt);
/**
 * Return the effective level. The effective level is the level 
 * of this logger, if it has one, or the level of the nearest 
 * ancestor to this logger that has a set level. The root 
 * logger's level is always set. 
 * 
 * @param[in] lgr the logger
 * @param[out] lvl the effective level
 * @return a value from @ref return_code.h indicating success or
 *         failure
*/
CHUCHO_EXPORT chucho_rc chucho_lgr_get_effective_level(const chucho_logger* lgr, const chucho_level** lvl);
/**
 * Return the level of a logger. If the level of the given 
 * logger is not set, then the lvl parameters will be set to 
 * NULL. If you wish to know what level this logger actually 
 * uses when writing events, please use @ref 
 * chucho_lgr_get_effective_level(). 
 * 
 * @param lgr the logger
 * @param lvl the level or NULL
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_get_level(const chucho_logger* lgr, const chucho_level** lvl);
/**
 * Return the name of a logger.

 * @param lgr the logger
 * @param nm the name
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_get_name(const chucho_logger* lgr, const char** nm);
/**
 * Get all writers associated with a logger. The buf parameter 
 * is an array of @ref chucho_writer pointers. The size of the 
 * array is buf_size, and the count parameter will tell you how 
 * many writers there actually are. You may pass NULL for buf if 
 * you are only interested in finding out how many writers you 
 * will need in buf. 
 * 
 * @post Ownership of all writers returned in buf is transferred 
 *       to the caller, and they must be released with the @ref
 *       chucho_release_writer() function.
 * 
 * @param[in] lgr the logger
 * @param[out] buf the array to receive the writers
 * @param[in] buf_size the number of elements in the array of 
 *       buf
 * @param[out] count the number of writers the logger actually 
 *       has
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_get_writers(const chucho_logger* lgr, chucho_writer** buf, size_t buf_size, size_t* count);
/**
 * Return whether a logger permits the given level. If the level 
 * is greater than or equal to the effective level of the 
 * logger, then it is permitted. 
 * 
 * @param[in] lgr the logger
 * @param[in] lvl the level to test
 * @param[out] state non-zero if the level is permitted
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_permits(const chucho_logger* lgr, const chucho_level* lvl, int* state);
/**
 * @copydoc chucho::logger::remove_all_writers
 * 
 * @param[in] lgr the logger
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_remove_all_writers(chucho_logger* lgr);
/**
 * Remove a specific writer from a logger.
 * 
 * @param[in] lgr the logger
 * @param[in] wrt the writer to remove
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_remove_writer(chucho_logger* lgr, chucho_writer* wrt);
/**
 * Set the level of a logger.
 * 
 * @param[in] lgr the logger
 * @param[in] lvl the level to set
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_set_level(chucho_logger* lgr, const chucho_level* lvl);
/**
 * Set whether this logger writes to its ancestors. When a 
 * logger writes to its ancestors, it will first write an 
 * incoming event to its writers, and then forward the event to 
 * its parent for writing. If the parent also is configured to 
 * write to ancestors, then once it has written the event, it 
 * will forward the event to its parent for writing. 
 *  
 * @note By default loggers do write to their ancestors.
 * 
 * @param[in] lgr the logger
 * @param[in] state non-zero if the logger should write to its ancestors
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_set_writes_to_ancestors(chucho_logger* lgr, int state);
/**
 * Return whether a logger writes to its ancestors.
 *  
 * @sa chucho_lgr_set_writes_to_ancestors 
 * 
 * @param[in] lgr the logger
 * @param[out] state non-zero if the logger writes to its 
 *       ancestors
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_lgr_writes_to_ancestors(const chucho_logger* lgr, int* state);

#if defined(__cplusplus)
}
#endif

#endif
