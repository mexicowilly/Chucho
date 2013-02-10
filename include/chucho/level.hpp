#if !defined(CHUCHO_LEVEL_HPP__)
#define CHUCHO_LEVEL_HPP__

#include <chucho/export.hpp>
#include <ostream>

namespace chucho
{

class level;

extern CHUCHO_EXPORT std::shared_ptr<level> TRACE_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> DEBUG_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> INFO_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> WARN_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> ERROR_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> FATAL_LEVEL;
extern CHUCHO_EXPORT std::shared_ptr<level> OFF_LEVEL;

/**
 * @defgroup levels
 * Levels provide the fundamental means by which a determination 
 * is made as to whether an event is logged or not. 
 */

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
 * @ingroup levels
 */
class CHUCHO_EXPORT level
{
public:
    /**
     * Figure out a level from its text. A case-insensitive 
     * comparison is performed to figure out which level a piece of 
     * text is talking about. For example, the text "eRroR" returns 
     * a shared pointer to an error_level. 
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

    /**
     * Return the name of the level.
     * 
     * @return the name
     */
    virtual const char* get_name() const = 0;
    /**
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

/**
 * @class trace_level level.hpp chucho/level.hpp
 * A type of level. Its name is TRACE and its value is 0.
 * @ingroup levels
 */
class CHUCHO_EXPORT trace_level : public level
{
public:
    /**
     * Return the name TRACE.
     * 
     * @return TRACE
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 0.
     * 
     * @return 0
     */
    virtual int get_value() const override;
};

/**
 * @class debug_level level.hpp chucho/level.hpp
 * A type of level. Its name is DEBUG and its value is 10000.
 * @ingroup levels
 */
class CHUCHO_EXPORT debug_level : public level
{
public:
    /**
     * Return the name DEBUG.
     * 
     * @return DEBUG
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 10000.
     * 
     * @return 10000
     */
    virtual int get_value() const override;
};

/**
 * @class info_level level.hpp chucho/level.hpp
 * A type of level. Its name is INFO and its value is 20000.
 * @ingroup levels
 */
class CHUCHO_EXPORT info_level : public level
{
public:
    /**
     * Return the name INFO.
     * 
     * @return INFO
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 20000.
     * 
     * @return 20000
     */
    virtual int get_value() const override;
};

/**
 * @class warn_level level.hpp chucho/level.hpp
 * A type of level. Its name is WARN and its value is 30000.
 * @ingroup levels
 */
class CHUCHO_EXPORT warn_level : public level
{
public:
    /**
     * Return the name WARN.
     * 
     * @return WARN
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 30000.
     * 
     * @return 30000
     */
    virtual int get_value() const override;
};

/**
 * @class error_level level.hpp chucho/level.hpp
 * A type of level. Its name is ERROR and its value is 40000.
 * @ingroup levels
 */
class CHUCHO_EXPORT error_level : public level
{
public:
    /**
     * Return the name ERROR.
     * 
     * @return ERROR
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 40000.
     * 
     * @return 40000
     */
    virtual int get_value() const override;
};

/**
 * @class fatal_level level.hpp chucho/level.hpp
 * A type of level. Its name is FATAL and its value is 50000.
 * @ingroup levels
 */
class CHUCHO_EXPORT fatal_level : public level
{
public:
    /**
     * Return the name FATAL.
     * 
     * @return FATAL
     */
    virtual const char* get_name() const override;
    /**
     * Return the value 50000.
     * 
     * @return 50000
     */
    virtual int get_value() const override;
};

/**
 * @class off_level level.hpp chucho/level.hpp
 * A type of level. Its name is OFF and its value is none of 
 * your business. Events of this level will never pass.
 * @ingroup levels
 */
class CHUCHO_EXPORT off_level : public level
{
public:
    /**
     * Return the name OFF.
     * 
     * @return OFF
     */
    virtual const char* get_name() const override;
    /**
     * Return a value that is none of your business.
     * 
     * @return none of your business
     */
    virtual int get_value() const override;
};

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
