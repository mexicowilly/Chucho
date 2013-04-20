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

#if !defined(CHUCHO_LEVEL_HPP__)
#define CHUCHO_LEVEL_HPP__

#include <chucho/export.hpp>
#include <chucho/syslog_constants.hpp>
#include <ostream>
#include <memory>

namespace chucho
{

/**
 * @class level level.hpp chucho/level.hpp
 * An event level. Levels allow the fundamental event filtration 
 * mechanism to occur. A @ref logger permits a certain level 
 * only if the level's value is greater than or equal to the 
 * effective level of the logger. The values of the predefined 
 * levels are as follows. 
 * <table> 
 *     <tr><th>Level</th><th>Value</th></tr>
 *     <tr><td>TRACE</td><td>0</td></tr>
 *     <tr><td>DEBUG</td><td>10000</td></tr>
 *     <tr><td>INFO</td><td>20000</td></tr>
 *     <tr><td>WARN</td><td>30000</td></tr>
 *     <tr><td>ERROR</td><td>40000</td></tr>
 *     <tr><td>FATAL</td><td>50000</td></tr>
 *     <tr><td>OFF</td><td>n/a</td></tr>
 * </table> 
 *  
 * You may create custom levels with subclasses of level, and 
 * you may insert your custom level anywhere you wish in the 
 * level hierarchy by setting its value. 
 *  
 * @ingroup levels 
 */
class CHUCHO_EXPORT level
{
public:
    /**
     * Trace level has a value of 0.
     */
    static std::shared_ptr<level> TRACE;
    /**
     * Debug level has a value of 10000.
     */
    static std::shared_ptr<level> DEBUG;
    /**
     * Info level has a value of 20000.
     */
    static std::shared_ptr<level> INFO;
    /**
     * Warn level has a value of 30000.
     */
    static std::shared_ptr<level> WARN;
    /**
     * Error level has a value of 40000.
     */
    static std::shared_ptr<level> ERROR;
    /**
     * Fatal level has a value of 50000.
     */
    static std::shared_ptr<level> FATAL;
    /**
     * Off level has a value that is none of your business.
     */
    static std::shared_ptr<level> OFF;

    /**
     * Figure out a level from its text. A case-insensitive 
     * comparison is performed to figure out which level a piece of 
     * text is talking about. For example, the text "eRroR" returns 
     * a shared pointer to equal to the @ref ERROR member.
     * 
     * @param text the text describing the level
     * @return the level
     * @throw exception if the text cannot be converted to a level
     */
    static std::shared_ptr<level> from_text(const std::string& text);

    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy the level.
     */
    virtual ~level();
    //@}

    /**
     * @name Operators
     */
    //@{
    /**
     * Is the value of this level equal to the value of another?
     * 
     * @param other the other level
     * @return true if the levels are equal
     */
    bool operator== (const level& other) const;
    /**
     * Is the value of this level not equal to the value of another?
     * 
     * @param other the other level
     * @return true if the levels are not equal
     */
    bool operator!= (const level& other) const;
    /**
     * Is the value of this level less than the value of another?
     * 
     * @param other the other level
     * @return true if this level's value is less than the other's
     */
    bool operator< (const level& other) const;
    /**
     * Is the value of this level less than or equal to the value of
     * another? 
     * 
     * @param other the other level
     * @return true if this level's value is less than or equal to 
     *         the other's
     */
    bool operator<= (const level& other) const;
    /**
     * Is the value of this level greater than the value of another?
     * 
     * @param other the other level
     * @return true if this level's value is greater than the 
     *         other's
     */
    bool operator> (const level& other) const;
    /**
     * Is the value of this level greater than or equal to the value
     * of another? 
     * 
     * @param other the other level
     * @return true if this level's value is greater than or equal 
     *         to the other's
     */
    bool operator>= (const level& other) const;
    //@}

    /**
     * Return the name of the level.
     * 
     * @return the name
     */
    virtual const char* get_name() const = 0;
    /**
     * Return the syslog severity that is associated with this log
     * level.
     * 
     * @return the severity
     */
    virtual syslog::severity get_syslog_severity() const = 0;
     */** 
     * Return the value of the level.
     * 
     * @return the value
     */
    virtual int get_value() const = 0;
};

/**
 * Write this level to a stream. The name of the level is 
 * written, but not the value. 
 * 
 * @relates level
 * @param stream the stream to which to write
 * @param lvl level to write
 * @return the stream
 */
CHUCHO_EXPORT std::ostream& operator<< (std::ostream& stream, const level& lvl);

inline bool level::operator== (const level& other) const
{
    return get_value() == other.get_value();
}

inline bool level::operator!= (const level& other) const
{
    return get_value() != other.get_value();
}

inline bool level::operator< (const level& other) const
{
    return get_value() < other.get_value();
}

inline bool level::operator<= (const level& other) const
{
    return get_value() <= other.get_value();
}

inline bool level::operator> (const level& other) const
{
    return get_value() > other.get_value();
}

inline bool level::operator>= (const level& other) const
{
    return get_value() >= other.get_value();
}

}

#endif
