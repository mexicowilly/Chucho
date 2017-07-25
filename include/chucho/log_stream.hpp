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

#if !defined(CHUCHO_LOG_STREAM_HPP__)
#define CHUCHO_LOG_STREAM_HPP__

#include <chucho/log_streambuf.hpp>
#include <chucho/level.hpp>
#include <chucho/function_name.hpp>
#include <chucho/non_copyable.hpp>
#include <ostream>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

/**
 * @file 
 * Classes, I/O manipulators and macros for streaming to 
 * loggers. 
 *  
 * @ingroup streams 
 */

namespace chucho
{

/** 
 * @class log_stream_base log_stream.hpp chucho/log_stream.hpp 
 * Base class for log streams. This only 
 * exists to ensure that initialization happens in the right 
 * order. You don't care about this class. These are not the 
 * droids you're looking for. 
 *
 * @ingroup streams
 */
class log_stream_base : non_copyable
{
protected:
    log_stream_base(std::shared_ptr<logger> lgr);
    
    log_streambuf buf_;
};

/**
 * @class log_stream log_stream.hpp chucho/log_stream.hpp
 * Stream to a log. This stream can be used to write to a given 
 * log. The I/O manipulators @ref trace, @ref debug, @ref info, 
 * @ref warn, @ref error, @ref fatal and @ref chucho::set_level, 
 * or the method @ref log_stream::set_level can used to set the 
 * level of the stream. The stream's level is stored even after 
 * the log message is written. So, if there are four messages in 
 * a row at @ref level::INFO_ level, then the level may just be 
 * set for the first. Additionally, a marker may be added to the
 * current message using the I/O manipulator @ref set_marker. 
 *  
 * The macro @ref CHUCHO_M should be used to start a log 
 * message. This macro does nothing more than set the location 
 * information of the current message into the stream, so if the 
 * location is not important, the macro can be omitted. 
 *  
 * The I/O manipulator @ref endm must be used to flush the 
 * message to the @ref logger. 
 *  
 * Typical usage is as follows. 
 * @code 
 * chucho::log_stream stream(my_logger); 
 * CHUCHO_M(stream) << chucho::info << "My dog has fleas." << chucho::endl << chucho::endm; 
 * @endcode 
 *  
 * Bear in mind that the performance of streaming to a log can 
 * be worse than the performance using the log macros, such as 
 * @ref CHUCHO_INFO. The reason is that when using the log 
 * macros, the arguments are are only evaluated if the message 
 * can actually be logged, given its level. When using the 
 * stream API, the arguments to build the message are always 
 * evaluated. 
 * 
 * @ingroup streams
 */
class CHUCHO_EXPORT log_stream : public log_stream_base,
                                 public std::ostream
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a log stream. All messages will be sent to the 
     * given logger. If no level is provided in the constructor, 
     * then the level must be set using @ref log_stream::set_level 
     * or one of the manipulators @ref trace, @ref debug, @ref info,
     * @ref warn, @ref error, @ref fatal or @ref chucho::set_level. 
     * If @ref endm is used to flush a message before any level has
     * been set, then the message is dropped. 
     * 
     * @param lgr the logger to which to write
     * @param lvl the level
     */
    log_stream(std::shared_ptr<logger> lgr,
               std::shared_ptr<level> lvl = std::shared_ptr<level>());
    /** @} */
    
    /**
     * Return the level of this stream.
     * @return the level
     */
    std::shared_ptr<level> get_level() const;
    /**
     * Set the level. In addition to this method, you may use the 
     * I/O manipulators @ref trace, @ref debug, @ref info, @ref 
     * warn, @ref error, @ref fatal and @ref chucho::set_level. 
     * 
     * @param lvl the level at which this stream will operate
     */
    void set_level(std::shared_ptr<level> lvl);
};

#if !defined(CHUCHO_DONT_DOCUMENT)

struct CHUCHO_EXPORT set_level_type
{
    set_level_type(std::shared_ptr<level> lvl) : level_(lvl) { }
    std::shared_ptr<level> level_;
};

CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, set_level_type sl);

struct CHUCHO_EXPORT set_marker_type
{
    set_marker_type(const marker& mrk) : marker_(mrk) { }
    marker marker_;
};

CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, set_marker_type sm);

#endif

/**
 * @def CHUCHO_M(stream) 
 * Prepare a stream to write a message. This macro puts the 
 * location information of the message, file, line number and 
 * function name, into the stream. If this information is not 
 * required, then the macro is not necessary. Typical usage is: 
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::info << "My dog has fleas." << chucho::endm; 
 * @endcode 
 */
// IMPORTANT: Leave the spaces around the type below. Gcc 4.7
// chokes when it sees <:.
#define CHUCHO_M(stream) \
    static_cast< ::chucho::log_streambuf* >((stream).rdbuf())->set_location(__FILE__, __LINE__, CHUCHO_FUNCTION_NAME); \
    (stream)

/**
 * An I/O manipulator to set the level of a @ref log_stream. All
 * levels that ship with Chucho have their own manipulators, so 
 * this manipulator is only required for custom levels. Typical 
 * usage is: 
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::set_level(chucho::level::INFO_()) << "My dog has fleas." << chucho::endm; 
 * @endcode 
 * 
 * @param lvl the level
 * @return an opaque type that you don't care about
 *
 * @sa trace, debug, info, warn, error, fatal
 */
inline set_level_type set_level(std::shared_ptr<level> lvl)
{
    return set_level_type(lvl);
}

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::DEBUG_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::debug << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& debug(std::ostream& ls)
{
    ls << set_level(level::DEBUG_());
    return ls;
}

/**
 * An I/O manipulator to write a line ending to a @ref log_stream.
 * This differs from std::endl in that a platform-appropriate line
 * ending is always written and the stream is not flushed.
 *
 * There is no need to call this function directly. Typical 
 * usage is: 
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::debug << "My dog has fleas." << chucho::endl << chucho::endm;
 * @endcode 
 *
 * @deprecated There should be no need to use this manipulator. The
 * newline can be inserted into the message as it normally would with
 * the %%n @ref pattern_formatter token.
 */
CHUCHO_EXPORT std::ostream& endl(std::ostream& ls);

/**
 * An I/O manipulator to flush a message to a @ref log_stream.
 * There is no need to call this function directly. Typical 
 * usage is: 
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::debug << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
CHUCHO_EXPORT std::ostream& endm(std::ostream& ls);

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::ERROR_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::error << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& error(std::ostream& ls)
{
    ls << set_level(level::ERROR_());
    return ls;
}

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::FATAL_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::fatal << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& fatal(std::ostream& ls)
{
    ls << set_level(level::FATAL_());
    return ls;
}

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::INFO_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::info << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& info(std::ostream& ls)
{
    ls << set_level(level::INFO_());
    return ls;
}

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::TRACE_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::trace << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& trace(std::ostream& ls)
{
    ls << set_level(level::TRACE_());
    return ls;
}

/**
 * An I/O manipulator to set the level of a @ref log_stream to 
 * @ref level::WARN_. There is no need to call this function 
 * directly. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::warn << "My dog has fleas." << chucho::endm; 
 * @endcode 
 *
 * @param ls the log stream
 * @return the log stream
 */
inline std::ostream& warn(std::ostream& ls)
{
    ls << set_level(level::WARN_());
    return ls;
}

/**
 * An I/O manipulator to set a marker into the current message 
 * of a @ref log_stream. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * chucho::marker mrk("grumpy");
 * CHUCHO_M(stream) << chucho::info << "My dog has fleas." << chucho::set_marker(mrk) << chucho::endm; 
 * @endcode 
 *
 * @param mrk the marker
 * @return an opaque type that you don't care about
 */
inline set_marker_type set_marker(const marker& mrk)
{
    return set_marker_type(mrk);
}

/**
 * An I/O manipulator to set a marker into the current message 
 * of a @ref log_stream. Typical usage is:
 * @code 
 * chucho::log_stream stream(lgr); 
 * CHUCHO_M(stream) << chucho::info << "My dog has fleas." << chucho::set_marker("crikey") << chucho::endm; 
 * @endcode 
 *
 * @param mrk the marker
 * @return an opaque type that you don't care about
 */
inline set_marker_type set_marker(const std::string& mrk)
{
    return set_marker_type(marker(mrk));
}

inline std::shared_ptr<level> log_stream::get_level() const
{
    return buf_.get_level();
}

inline void log_stream::set_level(std::shared_ptr<level> lvl)
{
    buf_.set_level(lvl);
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
