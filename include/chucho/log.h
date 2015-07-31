/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_LOG_H__)
#define CHUCHO_LOG_H__

/**
 * @file 
 * Logging macros functions that use the Chucho C API. These 
 * macros handle the machinery of filtering messages by level 
 * and adding markers. 
 *  
 * @ingroup c_loggers 
 */

#include <chucho/logger.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#if !defined(CHUCHO_DONT_DOCUMENT) && !defined(CHUCHO_FUNCTION_NAME)
#if defined(__GNUC__) || defined(__clang__) || defined(__SUNPRO_CC)
#define CHUCHO_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define CHUCHO_FUNCTION_NAME __FUNCTION__
#endif
#endif

/**
 * Log an event. 
 *  
 * @note This function is used by the C logging macros. You 
 *       probably don't need to call it yourself.
 * 
 * @param[in] lvl the level
 * @param[in] lgr the logger
 * @param[in] file the file name
 * @param[in] line the line number
 * @param[in] func the function name
 * @param[in] format the printf-style format
 * @param[in] ... any printf-style format parameters
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_log(const chucho_level* lvl,
                                   chucho_logger* lgr,
                                   const char* const file,
                                   int line,
                                   const char* const func,
                                   const char* const format, ...);

/**
 * Log an event with a marker.
 *  
 * @note This function is used by the C logging macros. You 
 *       probably don't need to call it yourself.
 * 
 * @param[in] lvl the level
 * @param[in] lgr the logger
 * @param[in] file the file name
 * @param[in] line the line number
 * @param[in] func the function name
 * @param[in] mark the marker
 * @param[in] format the printf-style format
 * @param[in] ... any printf-style format parameters
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_log_mark(const chucho_level* lvl,
                                        chucho_logger* lgr,
                                        const char* const file,
                                        int line,
                                        const char* const func,
                                        const char* const mark,
                                        const char* const format, ...);

/**
 * @def CHUCHO_C_TRACE(lgr, ...) 
 * Log a trace level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_TRACE(lgr, ...) chucho_log(chucho_trace_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_TRACE_M(lgr, mrk, ...) 
 * Log a trace level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_TRACE_M(lgr, mrk, ...) chucho_log_mark(chucho_trace_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_DEBUG(lgr, ...) 
 * Log a debug level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_DEBUG(lgr, ...) chucho_log(chucho_debug_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_DEBUG_M(lgr, mrk, ...) 
 * Log a debug level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_DEBUG_M(lgr, mrk, ...) chucho_log_mark(chucho_debug_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_INFO(lgr, ...) 
 * Log a info level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_INFO(lgr, ...) chucho_log(chucho_info_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_INFO_M(lgr, mrk, ...) 
 * Log a info level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_INFO_M(lgr, mrk, ...) chucho_log_mark(chucho_info_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_WARN(lgr, ...) 
 * Log a warn level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_WARN(lgr, ...) chucho_log(chucho_warn_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_WARN_M(lgr, mrk, ...) 
 * Log a warn level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_WARN_M(lgr, mrk, ...) chucho_log_mark(chucho_warn_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_ERROR(lgr, ...) 
 * Log a error level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_ERROR(lgr, ...) chucho_log(chucho_error_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_ERROR_M(lgr, mrk, ...) 
 * Log a error level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_ERROR_M(lgr, mrk, ...) chucho_log_mark(chucho_error_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_FATAL(lgr, ...) 
 * Log a fatal level message. 
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_FATAL(lgr, ...) chucho_log(chucho_fatal_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_FATAL_M(lgr, mrk, ...) 
 * Log a fatal level message with a marker.
 *  
 * @param lgr the logger, which must be of type @ref 
 *            chucho_logger*
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_FATAL_M(lgr, mrk, ...) chucho_log_mark(chucho_fatal_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)

#if defined(__cplusplus)
}
#endif

#endif
