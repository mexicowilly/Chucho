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

#if !defined(CHUCHO_LOG_H__)
#define CHUCHO_LOG_H__

#include <chucho/logger.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

#if !defined(CHUCHO_FUNCTION_NAME)
#if defined(__GNUC__) || defined(__clang__) || defined(__SUNPRO_CC)
#define CHUCHO_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define CHUCHO_FUNCTION_NAME __FUNCTION__
#endif
#endif

CHUCHO_EXPORT int chucho_log(const chucho_level* lvl,
                             chucho_logger* lgr,
                             const char* const file,
                             int line,
                             const char* const func,
                             const char* const format, ...);

CHUCHO_EXPORT int chucho_log_mark(const chucho_level* lvl,
                                  chucho_logger* lgr,
                                  const char* const file,
                                  int line,
                                  const char* const func,
                                  const char* const mark,
                                  const char* const format, ...);

#define CHUCHO_C_TRACE(lgr, ...) chucho_log(chucho_trace_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_TRACE_M(lgr, mrk, ...) chucho_log_mark(chucho_trace_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
#define CHUCHO_C_DEBUG(lgr, ...) chucho_log(chucho_debug_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_DEBUG_M(lgr, mrk, ...) chucho_log_mark(chucho_debug_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
#define CHUCHO_C_INFO(lgr, ...) chucho_log(chucho_info_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_INFO_M(lgr, mrk, ...) chucho_log_mark(chucho_info_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
#define CHUCHO_C_WARN(lgr, ...) chucho_log(chucho_warn_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_WARN_M(lgr, mrk, ...) chucho_log_mark(chucho_warn_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
#define CHUCHO_C_ERROR(lgr, ...) chucho_log(chucho_error_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_ERROR_M(lgr, mrk, ...) chucho_log_mark(chucho_error_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)
#define CHUCHO_C_FATAL(lgr, ...) chucho_log(chucho_fatal_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, __VA_ARGS__)
#define CHUCHO_C_FATAL_M(lgr, mrk, ...) chucho_log_mark(chucho_fatal_level(), (lgr), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, (mrk), __VA_ARGS__)

#if defined(__cplusplus)
}
#endif

#endif
