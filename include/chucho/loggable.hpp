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

#if !defined(CHUCHO_LOGGABLE_HPP__)
#define CHUCHO_LOGGABLE_HPP__

#include <chucho/logger.hpp>
#include <type_traits>

namespace chucho
{

/**
 * @class loggable loggable.hpp chucho/loggable.hpp
 * A class that inherits from loggable gets a logger associated 
 * with each instance for free. This gives you the ability to 
 * use the log macros tagged with @c LGBL in order to avoid 
 * having to manage the logger and what it might be called. For 
 * example, 
 * @code 
 * CHUCHO_INFO_LGBL("I just found " << num << " cats"); 
 * @endcode 
 * Instead of, 
 * @code 
 * CHUCHO_INFO(where_is_my_logger(), "I just found " << num << " cats"); 
 * @endcode 
 * By default the logger is named for the class (type template 
 * argument). If your class is called @c my::cool::stuff, then 
 * your logger will be named 
 * @c my.cool.stuff. 
 *  
 * @ingroup miscellaneous 
 */
template <typename type>
class loggable
{
protected:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a loggable giving the name of type to the logger. 
     * For example, if your type is called @c my::cool::stuff, then 
     * the logger will have the name @c my.cool.stuff. 
     */
    loggable();
    /**
     * Construct a loggable using the given name for the name of the 
     * logger. 
     * 
     * @param name the name of the logger
     */
    loggable(const std::string& name);
    //@}

    /**
     * Return the logger for this instance.
     * 
     * @return the logger
     */
    std::shared_ptr<logger> get_logger() const;
    /**
     * Rename the logger. This method is meant to be used in cases 
     * where a subclass of a subclass of loggable needs a different 
     * logger name. Say you have a class hierarchy built off the 
     * abstract class shape. You make shape a subclass of loggable, 
     * so that everyone gets a logger. Subclasses of shape don't 
     * want their loggers to be named shape, so they rename the 
     * logger in the subclass' constructor. 
     * 
     * @param new_type the type on which to base the new name
     */
    virtual void rename_logger(const std::type_info& new_type);
    /**
     * Rename the logger. This method is meant to be used in cases 
     * where a subclass of a subclass of loggable needs a different 
     * logger name. Say you have a class hierarchy built off the 
     * abstract class shape. You make shape a subclass of loggable, 
     * so that everyone gets a logger. Subclasses of shape don't 
     * want their loggers to be named shape, so they rename the 
     * logger in the subclass' constructor. 
     * 
     * @param name the new logger name
     */
    virtual void rename_logger(const std::string& name);

private:
    std::shared_ptr<logger> logger_;
};

template <typename type>
loggable<type>::loggable()
    : logger_(logger::get(logger::type_to_logger_name(typeid(type))))
{
}

template <typename type>
loggable<type>::loggable(const std::string& name)
    : logger_(logger::get(name))
{
}

template <typename type>
std::shared_ptr<logger> loggable<type>::get_logger() const
{
    return logger_;
}

template <typename type>
void loggable<type>::rename_logger(const std::type_info& new_type)
{
    logger_ = logger::get(logger::type_to_logger_name(new_type));
}

template <typename type>
void loggable<type>::rename_logger(const std::string& name)
{
    logger_ = logger::get(name);
}

}

#endif
