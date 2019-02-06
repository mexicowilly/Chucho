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

#if !defined(CHUCHO_LOG_STREAMBUF_HPP__)
#define CHUCHO_LOG_STREAMBUF_HPP__

#include <chucho/logger.hpp>
#include <chucho/marker.hpp>
#include <streambuf>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace chucho
{

/**
 * @class log_streambuf log_streambuf.hpp chucho/log_streambuf.hpp 
 * A standard streambuf that writes to a @ref logger. This 
 * streambuf is only suitable for output, and does not support 
 * any input functionality. It is intended for use with @ref 
 * log_stream and the associated I/O manipulators, but it may 
 * have some limited use outside of that context. 
 *  
 * @ingroup streams 
 */
class CHUCHO_EXPORT log_streambuf : public std::streambuf,
                                    public status_reporter
{
public:
    /**
     * @name Constructor 
     * @{ 
     */
    /**
     * Construct a streambuf.
     * 
     * @param lgr the logger to which to write
     */
    log_streambuf(std::shared_ptr<logger> lgr);
    /** @}  */

    /**
     * Flush the buffer to the @ref logger. This method is invoked 
     * by the I/O manipulator @ref endm. If no level has been set 
     * for this streambuf, then the message is dropped. 
     *  
     * This method also resets the message, the location information 
     * and the marker. 
     */
    void flush_message();
    /**
     * Return the level of this streambuf.
     * 
     * @return the level
     */
    std::shared_ptr<level> get_level() const;
    /**
     * Return the logger of this streambuf.
     *
     * @return the logger
     */
    std::shared_ptr<logger> get_logger() const;
    /**
     * Used internally by the stream to add characters to a message. 
     * You don't use this method. Don't even look at it.
     * 
     * @param ch the character to add
     * @return the same character of the argument ch
     */
    virtual int_type overflow(int_type ch) override;
    /**
     * Set the level of this streambuf. This is invoked by the @ref 
     * log_stream method and by all of the I/O manipulators that can 
     * set the level. 
     * 
     * @param lvl the level 
     * @sa trace, debug, info, warn, error, fatal, chucho::set_level
     */
    void set_level(std::shared_ptr<level> lvl);
    /**
     * Set the location of the current message. This is invoked by 
     * the macro @ref CHUCHO_M. 
     * 
     * @param file_name the file
     * @param line_number the line
     * @param function_name the function
     */
    void set_location(const char* const file_name,
                      unsigned line_number,
                      const char* const function_name);
    /**
     * Set the marker. This is invoked by the I/O manipulator @ref 
     * chucho::set_marker. 
     * 
     * @param mrk the marker
     */
    void set_marker(const marker& mrk);
    
private:
    std::shared_ptr<logger> logger_;
    std::string message_;
    std::shared_ptr<level> level_;
    optional<marker> marker_;
    const char* file_name_;
    unsigned line_number_;
    const char* function_name_;
};

inline std::shared_ptr<level> log_streambuf::get_level() const
{
    return level_;
}

inline std::shared_ptr<logger> log_streambuf::get_logger() const
{
    return logger_;
}

inline void log_streambuf::set_level(std::shared_ptr<level> lvl)
{
    level_ = lvl;
}

inline void log_streambuf::set_marker(const marker& mrk)
{
    marker_ = mrk;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
