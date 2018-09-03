/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_BUILD) && defined(__cplusplus)
#error "When using C++, you want the header log.hpp"
#endif

#include <chucho/logger.h>

/**
 * @file 
 * Logging macros functions that use the Chucho C API. These 
 * macros handle the machinery of filtering messages by level 
 * and adding markers. 
 *  
 * @ingroup c_loggers 
 */

#if defined(__cplusplus)
extern "C"
{
#endif

#if !defined(CHUCHO_DONT_DOCUMENT) && !defined(CHUCHO_FUNCTION_NAME)
#if defined(__GNUC__) || defined(__clang__) || defined(__SUNPRO_C)
#define CHUCHO_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define CHUCHO_FUNCTION_NAME __FUNCTION__
#endif

#define CHUCHO_C_INTERNAL(lvl, lgr, fl, ln, fnc, ...) \
    _Generic((lgr), chucho_logger_t*: chucho_log_logger, default: chucho_log) \
    ((lvl), (lgr), (fl), (ln), (fnc), __VA_ARGS__)
#define CHUCHO_C_INTERNAL_M(lvl, lgr, fl, ln, fnc, mrk, ...) \
    _Generic((lgr), chucho_logger_t*: chucho_log_mark_logger, default: chucho_log_mark) \
    ((lvl), (lgr), (fl), (ln), (fnc), (mrk), __VA_ARGS__)

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
 */
CHUCHO_EXPORT void chucho_log(chucho_level_t lvl,
                              const char* const lgr,
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
 */
CHUCHO_EXPORT void chucho_log_mark(chucho_level_t lvl,
                                   const char* const lgr,
                                   const char* const file,
                                   int line,
                                   const char* const func,
                                   const char* const mark,
                                   const char* const format, ...);

CHUCHO_EXPORT void chucho_log_logger(chucho_level_t lvl,
                                     chucho_logger_t* lgr,
                                     const char* const file,
                                     int line,
                                     const char* const func,
                                     const char* const format, ...);
CHUCHO_EXPORT void chucho_log_mark_logger(chucho_level_t lvl,
                                          chucho_logger_t* lgr,
                                          const char* const file,
                                          int line,
                                          const char* const func,
                                          const char* const mark,
                                          const char* const format, ...);
/**
 * @def CHUCHO_C_TRACE(lgr, ...) 
 * Log a trace level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_TRACE(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_TRACE, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_TRACE_M(lgr, mrk, ...) 
 * Log a trace level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_TRACE_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_TRACE, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_DEBUG(lgr, ...) 
 * Log a debug level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_DEBUG(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_DEBUG, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_DEBUG_M(lgr, mrk, ...) 
 * Log a debug level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_DEBUG_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_DEBUG, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_INFO(lgr, ...) 
 * Log a info level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_INFO(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_INFO, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_INFO_M(lgr, mrk, ...) 
 * Log a info level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_INFO_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_INFO, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_WARN(lgr, ...) 
 * Log a warn level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_WARN(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_WARN, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_WARN_M(lgr, mrk, ...) 
 * Log a warn level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_WARN_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_WARN, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_ERROR(lgr, ...) 
 * Log a error level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_ERROR(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_ERROR, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_ERROR_M(lgr, mrk, ...) 
 * Log a error level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_ERROR_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_ERROR, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
/**
 * @def CHUCHO_C_FATAL(lgr, ...) 
 * Log a fatal level message. 
 *  
 * @param lgr the logger
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_FATAL(lgr, ...) CHUCHO_C_INTERNAL(CHUCHO_FATAL, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
/**
 * @def CHUCHO_C_FATAL_M(lgr, mrk, ...) 
 * Log a fatal level message with a marker.
 *  
 * @param lgr the logger
 * @param mrk the marker, which must be of type char*
 * @param ... printf-style parameters that must include the
 *        format text first
 */
#define CHUCHO_C_FATAL_M(lgr, mrk, ...) CHUCHO_C_INTERNAL_M(CHUCHO_FATAL, (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)

#if defined(__cplusplus)
}
#endif

#endif
