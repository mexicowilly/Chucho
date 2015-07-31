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

#if !defined(CHUCHO_LOG_HPP__)
#define CHUCHO_LOG_HPP__

/**
 * @file
 * Macros for writing messages to logs. These macros handle the 
 * machinery of filtering messages by level and adding markers. 
 *  
 * @ingroup loggers 
 */

#include <chucho/logger.hpp>
#include <chucho/function_name.hpp>
#include <sstream>

#if !defined(CHUCHO_DONT_DOCUMENT)

#define CHUCHO_LOG(lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl)) \
        { \
            std::ostringstream __chucho_internal_stream; \
            __chucho_internal_stream << msg; \
            (lg)->write(::chucho::event((lg), (lvl), __chucho_internal_stream.str(), (fl), (ln), (fnc))); \
        } \
    } while (false)

#define CHUCHO_LOG_STR(lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl)) \
            (lg)->write(::chucho::event((lg), (lvl), (msg), (fl), (ln), (fnc))); \
    } while (false)

#define CHUCHO_LOG_M(mrk, lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl)) \
        { \
            std::ostringstream __chucho_internal_stream; \
            __chucho_internal_stream << msg; \
            (lg)->write(::chucho::event((lg), (lvl), __chucho_internal_stream.str(), (fl), (ln), (fnc), (mrk))); \
        } \
    } while (false)

#define CHUCHO_LOG_STR_M(mrk, lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl)) \
            (lg)->write(::chucho::event((lg), (lvl), (msg), (fl), (ln), (fnc), (mrk))); \
    } while (false)

#define CHUCHO_INTERNAL_LOG(lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG(::chucho::level::lvl(), lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_STR(lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_STR(::chucho::level::lvl(), lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_M(mrk, lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_M(mrk, ::chucho::level::lvl(), lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_STR_M(mrk, lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_STR_M(mrk, ::chucho::level::lvl(), lg, fl, ln, fnc, msg)

#endif

/**
 * @def CHUCHO_TRACE(lg, msg)
 * Log a trace-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE(lg, msg) CHUCHO_INTERNAL_LOG(TRACE_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_LGBL(msg)
 * Log a trace-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE_LGBL(msg) CHUCHO_INTERNAL_LOG(TRACE_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_STR(lg, msg)
 * Log a trace-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(TRACE_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_LGBL_STR(msg)
 * Log a trace-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(TRACE_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_M(mrk, lg, msg)
 * Log a trace-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger>
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, TRACE_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_LGBL_M(mrk, msg)
 * Log a trace-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, TRACE_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_STR_M(mrk, lg, msg)
 * Log a trace-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, TRACE_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_LGBL_STR_M(mrk, msg)
 * Log a trace-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, TRACE_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG(lg, msg)
 * Log a debug-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG(lg, msg) CHUCHO_INTERNAL_LOG(DEBUG_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_LGBL(msg)
 * Log a debug-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG_LGBL(msg) CHUCHO_INTERNAL_LOG(DEBUG_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_STR(lg, msg)
 * Log a debug-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(DEBUG_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_LGBL_STR(msg)
 * Log a debug-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(DEBUG_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_M(mrk, lg, msg)
 * Log a debug-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, DEBUG_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_LGBL_M(mrk, msg)
 * Log a debug-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, DEBUG_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_STR_M(lg, msg)
 * Log a debug-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, DEBUG_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_LGBL_STR_M(mrk, msg)
 * Log a debug-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, DEBUG_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO(lg, msg)
 * Log a info-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO(lg, msg) CHUCHO_INTERNAL_LOG(INFO_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_LGBL(msg)
 * Log a info-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO_LGBL(msg) CHUCHO_INTERNAL_LOG(INFO_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_STR(lg, msg)
 * Log a info-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(INFO_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_LGBL_STR(msg)
 * Log a info-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(INFO_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_M(mrk, lg, msg)
 * Log a info-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, INFO_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_LGBL_M(mrk, msg)
 * Log a info-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, INFO_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_STR_M(lg, msg)
 * Log a info-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, INFO_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_LGBL_STR_M(mrk, msg)
 * Log a info-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, INFO_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN(lg, msg)
 * Log a warn-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN(lg, msg) CHUCHO_INTERNAL_LOG(WARN_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_LGBL(msg)
 * Log a warn-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN_LGBL(msg) CHUCHO_INTERNAL_LOG(WARN_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_STR(lg, msg)
 * Log a warn-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(WARN_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_LGBL_STR(msg)
 * Log a warn-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(WARN_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_M(mrk, lg, msg)
 * Log a warn-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, WARN_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_LGBL_M(mrk, msg)
 * Log a warn-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, WARN_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_STR_M(lg, msg)
 * Log a warn-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, WARN_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_LGBL_STR_M(mrk, msg)
 * Log a warn-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, WARN_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR(lg, msg)
 * Log a error-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR(lg, msg) CHUCHO_INTERNAL_LOG(ERROR_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_LGBL(msg)
 * Log a error-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR_LGBL(msg) CHUCHO_INTERNAL_LOG(ERROR_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_STR(lg, msg)
 * Log a error-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(ERROR_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_LGBL_STR(msg)
 * Log a error-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(ERROR_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_M(mrk, lg, msg)
 * Log a error-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, ERROR_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_LGBL_M(mrk, msg)
 * Log a error-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, ERROR_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_STR_M(lg, msg)
 * Log a error-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, ERROR_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_LGBL_STR_M(mrk, msg)
 * Log a error-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, ERROR_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL(lg, msg)
 * Log a fatal-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL(lg, msg) CHUCHO_INTERNAL_LOG(FATAL_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_LGBL(msg)
 * Log a fatal-level message from inside a subclass of @ref 
 * chucho::loggable. The logger used is taken from the current chucho::loggable 
 * instance. 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL_LGBL(msg) CHUCHO_INTERNAL_LOG(FATAL_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_STR(lg, msg)
 * Log a fatal-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(FATAL_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_LGBL_STR(msg)
 * Log a fatal-level message that is text only from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance. 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_LGBL_STR(msg) CHUCHO_INTERNAL_LOG_STR(FATAL_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_M(mrk, lg, msg)
 * Log a fatal-level message with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, FATAL_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_LGBL_M(mrk, msg)
 * Log a fatal-level message with a marker from inside a 
 * subclass of @ref chucho::loggable. The logger used is taken from the 
 * current chucho::loggable instance.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL_LGBL_M(mrk, msg) CHUCHO_INTERNAL_LOG_M(mrk, FATAL_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_STR_M(lg, msg)
 * Log a fatal-level message that is text only with a marker.
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, FATAL_, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_LGBL_STR_M(mrk, msg)
 * Log a fatal-level message that is text only with a marker 
 * from inside a subclass of @ref chucho::loggable. The logger used is
 * taken from the current chucho::loggable instance. 
 * @param mrk the marker, which may either be a marker reference 
 *            or a piece of text (const char* or std::string
 *            reference), in which case a marker will be created
 *            on the fly
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_LGBL_STR_M(mrk, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, FATAL_, get_logger(), __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)

#endif
