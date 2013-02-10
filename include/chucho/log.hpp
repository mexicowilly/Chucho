#if !defined(CHUCHO_LOG_HPP__)
#define CHUCHO_LOG_HPP__

#include <chucho/logger.hpp>
#include <sstream>

/**
 * @file
 * Macros for writing messages to logs. These macros handle the 
 * machinery of filtering messages by level and adding markers. 
 *  
 * @ingroup loggers 
 */

#if !defined(CHUCHO_DONT_DOCUMENT)

#if defined(__GNUC__) || defiend(__clang__) || defined(__SUNPRO_CC)
#define CHUCHO_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define CHUCHO_FUNCTION_NAME __FUNCTION__
#endif


#define CHUCHO_LOG(lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits((lvl)) \
        { \
            std::ostringstream __chucho_internal_stream; \
            __chucho_internal_stream << msg; \
            (lg)->write(::chucho::event((lg), (lvl), __chucho_internal_stream.str(), (fl), (ln), (fnc))); \
        } \
    } while (false)

#define CHUCHO_LOG_STR(lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl()) \
            (lg)->write(::chucho::event((lg), (lvl), (msg), (fl), (ln), (fnc))); \
    } while (false)

#define CHUCHO_LOG_M(mrk, lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits((lvl)) \
        { \
            std::ostringstream __chucho_internal_stream; \
            __chucho_internal_stream << msg; \
            (lg)->write(::chucho::event((lg), (lvl), __chucho_internal_stream.str(), (fl), (ln), (fnc), (mrk))); \
        } \
    } while (false)

#define CHUCHO_LOG_STR_M(mrk, lvl, lg, fl, ln, fnc, msg) \
    do \
    { \
        if ((lg)->permits(lvl()) \
            (lg)->write(::chucho::event((lg), (lvl), (msg), (fl), (ln), (fnc), (mrk))); \
    } while (false)

#define CHUCHO_INTERNAL_LOG(lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG(::chucho::##lvl##_LEVEL, lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_STR(lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_STR(::chucho::##lvl##_level(), lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_M(mrk, lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_M(mrk, ::chucho::##lvl##_LEVEL, lg, fl, ln, fnc, msg)
#define CHUCHO_INTERNAL_LOG_STR_M(mrk, lvl, lg, fl, ln, fnc, msg) CHUCHO_LOG_STR_M(mrk, ::chucho::##lvl##_level(), lg, fl, ln, fnc, msg)

#endif

/**
 * @def CHUCHO_TRACE(lg, msg)
 * Log a trace-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE(lg, msg) CHUCHO_INTERNAL_LOG(TRACE, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_STR(lg, msg)
 * Log a trace-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(TRACE, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_M(mrk, lg, msg)
 * Log a trace-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_TRACE_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, TRACE, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_TRACE_STR_M(lg, msg)
 * Log a trace-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_TRACE_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, TRACE, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG(lg, msg)
 * Log a debug-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG(lg, msg) CHUCHO_INTERNAL_LOG(DEBUG, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_STR(lg, msg)
 * Log a debug-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(DEBUG, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_M(mrk, lg, msg)
 * Log a debug-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_DEBUG_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, DEBUG, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_DEBUG_STR_M(lg, msg)
 * Log a debug-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_DEBUG_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, DEBUG, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO(lg, msg)
 * Log a info-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO(lg, msg) CHUCHO_INTERNAL_LOG(INFO, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_STR(lg, msg)
 * Log a info-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(INFO, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_M(mrk, lg, msg)
 * Log a info-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_INFO_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, INFO, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_INFO_STR_M(lg, msg)
 * Log a info-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_INFO_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, INFO, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN(lg, msg)
 * Log a warn-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN(lg, msg) CHUCHO_INTERNAL_LOG(WARN, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_STR(lg, msg)
 * Log a warn-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(WARN, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_M(mrk, lg, msg)
 * Log a warn-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_WARN_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, WARN, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_WARN_STR_M(lg, msg)
 * Log a warn-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_WARN_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, WARN, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR(lg, msg)
 * Log a error-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR(lg, msg) CHUCHO_INTERNAL_LOG(ERROR, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_STR(lg, msg)
 * Log a error-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(ERROR, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_M(mrk, lg, msg)
 * Log a error-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_ERROR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, ERROR, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_ERROR_STR_M(lg, msg)
 * Log a error-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_ERROR_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, ERROR, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL(lg, msg)
 * Log a fatal-level message.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL(lg, msg) CHUCHO_INTERNAL_LOG(FATAL, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_STR(lg, msg)
 * Log a fatal-level message that is text only.
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_STR(lg, msg) CHUCHO_INTERNAL_LOG_STR(FATAL, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_M(mrk, lg, msg)
 * Log a fatal-level message with a marker.
 * @param mrk the marker, which must be a 
 *            std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which may be formatted for 
 *            output to a std::stream, like "I have " << 7 << "
 *            dreams."
 */
#define CHUCHO_FATAL_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_M(mrk, FATAL, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)
/**
 * @def CHUCHO_FATAL_STR_M(lg, msg)
 * Log a fatal-level message that is text only with a marker.
 * @param mrk the marker, which must be a std::shared_ptr<marker>
 * @param lg the logger, which must be a std::shared_ptr<logger> 
 * @param msg the message to write, which can only be a const 
 *            char* or a std::string
 */
#define CHUCHO_FATAL_STR_M(mrk, lg, msg) CHUCHO_INTERNAL_LOG_STR_M(mrk, FATAL, lg, __FILE__, __LINE__, CHUCHO_FUNCTION_NAME, msg)

#endif
