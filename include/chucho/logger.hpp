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

#if !defined(CHUCHO_LOGGER_HPP__)
#define CHUCHO_LOGGER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <list>

namespace chucho
{

/**
 * @class logger logger.hpp chucho/logger.hpp
 * A logger writes stuff when someone wants to log something. 
 * There is a hierarchy of loggers, which is to say that loggers 
 * have parents. The root of this hierarchy is a logger with an 
 * empty name that is also known as the root logger. 
 *  
 * A logger will carry its ancestry with it in its name, with 
 * the separators being dots ("."). For example, a logger named
 * "one.two.go" describes the last descendant of three loggers: 
 * "one", "two" and "go". The logger "one" is the parent of 
 * "two", and "two" is the parent of "go". There is an unseen 
 * ancestor in the hierarchy of "one.two.go", which is the 
 * unnamed root logger, so really there are four.
 *  
 * A logger's ancestors play an important role in how chucho 
 * functions. For example, not all loggers have explicitly set 
 * levels. A log level is one of the primary determining factors 
 * into whether a log event gets written or not. If a logger 
 * does not have a level set, then it inherits its level from 
 * the nearest ancestor that has a set level. The root logger 
 * always has a level. 
 *  
 * Additionally, loggers may have writers. If a logger is 
 * configured to write to its ancestors, then after it writes to 
 * its own writers, it will forward the event to its nearest 
 * ancestor, which will then write the event and forward it 
 * again, if it is configured to do so. So, loggers can inherit 
 * the writers of their ancestors, but they don't have to if 
 * they don't want to. 
 *  
 * @sa log.hpp for macros used in log event filtering 
 *  
 * @ingroup loggers
 */
class CHUCHO_EXPORT logger : non_copyable,
                             public std::enable_shared_from_this<logger>,
                             public status_reporter,
                             public configurable
{
public:
    /**
     * Get a logger. Remember that a logger carries its ancestry 
     * with it in its name, so you must specify all ancestors using 
     * dots (".") as separators. If any named ancestors don't exist, 
     * then they will be created automatically. The same goes for 
     * the final descendant in the chain. 
     *
     * @param name the dot-separated name of the logger
     * @return the logger
     */
    static std::shared_ptr<logger> get(const std::string& name);
    /**
     * Return all existing loggers. This returns all existing 
     * loggers, even ones that are not currently in use.
     *  
     * @return loggers in use
     */
    static std::vector<std::shared_ptr<logger>> get_existing_loggers();
   /**
    * Remove loggers not in use. When a logger is created it is 
    * held in memory so that subsequent retrievals will continue to 
    * get the same logger with the same configuration. This can 
    * leak memory if you create a lot of single-use loggers, for 
    * example. You can periodically recover the unused memory by 
    * calling this method. 
    */
    static void remove_unused_loggers();
    /**
     * Turn the name of a type into a logger name. If your type is 
     * @c one::two::go, then the logger name returned will be @c 
     * one.two.go.
     *  
     * This method is used internally in the @ref loggable class. 
     * You may or may not find it useful. 
     * 
     * @param info the type information
     * @return the logger name for the given type
     */
    static std::string type_to_logger_name(const std::type_info& info);

    virtual ~logger();
   /**
    * Add a writer. This writer will be written in subsequent event 
    * writes from this logger. 
    * 
    * @param wrt the writer
    * @throw std::invalid_argument if wrt is an uninitialized 
    *        std::unique_ptr
    */
    void add_writer(std::unique_ptr<writer>&& wrt);
    /**
     * Remove all writers from this logger.
     */
    void clear_writers();
   /**
    * Return the effective level. The effective level is the level 
    * of this logger, if it has one, or the level of the nearest 
    * ancestor to this logger that has a set level. The root 
    * logger's level is always set. 
    * 
    * @return the effective level
    */
    std::shared_ptr<level> get_effective_level() const;
   /**
    * Return the level of this logger. The level might not be set, 
    * in which case the returned level would be an uninitialized 
    * std::shared_ptr. To get the level at which this logger 
    * actually logs, please use 
    * @ref get_effective_level(). 
    * 
    * @sa @ref get_effective_level()
    * @return the level
    */
    std::shared_ptr<level> get_level();
    /**
     * Return the name of this logger. The name includes the full 
     * ancestry, like "one.two.go". 
     * 
     * @return the name
     */
    const std::string& get_name() const;
    /**
     * Return a writer.
     *
     * @param name the name of the writer
     * @throws std::invalid_argument if the writer does not exist
     */
    writer& get_writer(const std::string& name);
    /**
     * Return the name of the writers.
     * 
     * @return the writers' names
     */
    std::vector<std::string> get_writer_names();
    /**
     * Does this logger permit a level? If the level is greater than
     * or equal to the effective level of this logger, then the 
     * level is permitted. 
     * 
     * @param lvl the level to test
     * @return true if the level would be permitted by this logger
     */
    bool permits(std::shared_ptr<level> lvl);
    /**
     * Remove a specific writer from this logger.
     * 
     * @param wrt the name of the writer to remove
     */
    void remove_writer(const std::string& wrt);
    /**
     * Reset this logger to default settings. This method removes a 
     * set level, removes all writers, and sets 
     * writes_to_ancestors() to true. 
     *  
     * @note This method is used in configuration::reconfigure, and 
     *       is probably not generally useful.
     */
    void reset();
    /**
     * Set the level of this logger. To unset the level, you would 
     * pass an uninitialized std::shared_ptr as lvl. 
     * 
     * @param lvl the new level, which can be an uninitialized 
     *            std::shared_ptr
     */
    void set_level(std::shared_ptr<level> lvl);
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
     * @param val true if the logger should write to its ancestors
     */
    void set_writes_to_ancestors(bool val);
    /**
     * Unconditionally write an event. If you are worried about 
     * levels and filtering, please see the file @ref log.hpp. 
     * 
     * @param evt the event to write
     */
    void write(const event& evt);
    /**
     * Return whether this logger writes to its ancestors.
     * 
     * @sa @ref set_writes_to_ancestors()
     * 
     * @return true if this logger writes to its ancestors
     */
    bool writes_to_ancestors();

private:
    friend class logger_factory;

    static CHUCHO_NO_EXPORT std::shared_ptr<logger> get_impl(const std::string& name);
    static CHUCHO_NO_EXPORT void initialize();

    CHUCHO_NO_EXPORT logger(const std::string& name, std::shared_ptr<level> lvl = std::shared_ptr<level>());

    std::shared_ptr<logger> parent_;
    std::string name_;
    std::shared_ptr<level> level_;
    std::list<std::unique_ptr<writer>> writers_;
    std::mutex guard_;
    bool writes_to_ancestors_;
};

inline const std::string& logger::get_name() const
{
    return name_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
