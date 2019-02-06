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

#if !defined(CHUCHO_STREAMABLE_HPP__)
#define CHUCHO_STREAMABLE_HPP__

#include <chucho/loggable.hpp>
#include <chucho/log_stream.hpp>

/**
 * @file
 * Classes and macros for writing log events to
 * Chucho-supplied log streams and loggers.
 *
 * @ingroup streams
 */

namespace chucho
{

/**
 * @class streamable streamable.hpp chucho/streamable.hpp
 * A class that inherits from @ref streamable gets a @ref logger
 * and a @ref log_stream for free. In addition to being able to
 * write log events with the LGBL-tagged macros that are available
 * to descendants of @ref loggable, @ref CHUCHO_MS can be used to
 * write to the @ref log_stream that is provided. So, in non-static
 * methods of classes that inherit from @ref streamable, the
 * following may be used:
 * @code
 * CHUCHO_INFO_LGBL("All the " << animals << " like me");
 * @endcode
 * And:
 * @code
 * CHUCHO_MS << chucho::info << "All the " << animals << " like me" << chucho::endm;
 * @endcode
 * Both will have identical results.
 *
 * @ingroup streams
 */
template <typename type>
class streamable : public loggable<type>
{
protected:
    /**
     * @name Constructors
     */
    //@{
    /**
     * Construct a streamable whose logger's name will be
     * derived from the type. For example, if the type of
     * this class is three::doggies, then the logger will be
     * named three.doggies.
     *
     * @param lvl the initial level of the stream
     */
    streamable(std::shared_ptr<level> lvl = std::shared_ptr<level>());
    /**
     * Construct a streamable whose logger has the given name.
     *
     * @param name the name of the logger
     * @param lvl the initial level of the stream
     */
    streamable(const std::string& name,
               std::shared_ptr<level> lvl = std::shared_ptr<level>());
    /**
     * Copy a stream.
     *
     * @param other the stream to copy
     */
    streamable(const streamable& other);
    /**
     * Move a stream.
     *
     * @param other the stream to move
     */
    streamable(streamable&& other);
    //@}

    /**
     * @name Operators
     */
    //@{
    /**
     * Assign from another stream.
     *
     * @param other the stream to copy
     * @return this stream
     */
    streamable& operator= (const streamable& other);
    //@}

    /**
     * Return the log stream.
     *
     * @return the stream
     */
    log_stream& get_log_stream() const;
    virtual void rename_logger(const std::type_info& new_type) override;
    virtual void rename_logger(const std::string& name) override;

private:
    std::unique_ptr<log_stream> log_stream_;
};

/**
 * @def CHUCHO_MS
 * Prepare the internal stream to write a message. This macro puts the
 * location information of the message, file, line number and
 * function name, into the stream. If this information is not
 * required, then the macro is not necessary. Typical usage is:
 * @code
 * CHUCHO_MS << chucho::info << "My dog has fleas." << chucho::endm;
 * @endcode
 *
 * @note This macro may only be used in non-static methods of a class
 * that inherits from @ref streamable.
 */
#define CHUCHO_MS CHUCHO_M(get_log_stream())

template <typename type>
streamable<type>::streamable(std::shared_ptr<level> lvl)
    : log_stream_(new log_stream(loggable<type>::get_logger(), lvl))
{
}

template <typename type>
streamable<type>::streamable(const std::string& name,
                             std::shared_ptr<level> lvl)
    : loggable<type>(name),
      log_stream(new log_stream(loggable<type>::get_logger(), lvl))
{
}

template <typename type>
streamable<type>::streamable(const streamable& other)
    : log_stream_(new log_stream(loggable<type>::get_logger(), other.log_stream_->get_level()))
{
    loggable<type>::rename_logger(other.get_logger()->get_name());
}

template <typename type>
streamable<type>::streamable(streamable&& other)
    : loggable<type>(other.get_logger()->get_name()),
      log_stream_(std::move(other.log_stream_))
{
}

template <typename type>
streamable<type>& streamable<type>::operator= (const streamable& other)
{
    if (&other != this)
    {
        loggable<type>::rename_logger(other.get_logger()->get_name());
        log_stream_.reset(new log_stream(other.get_logger(), other.log_stream_->get_level()));
    }
    return *this;
}

template <typename type>
log_stream& streamable<type>::get_log_stream() const
{
    return *log_stream_;
}

template <typename type>
void streamable<type>::rename_logger(const std::type_info& new_type)
{
    loggable<type>::rename_logger(new_type);
    log_stream_.reset(new log_stream(loggable<type>::get_logger(), log_stream_->get_level()));
}

template <typename type>
void streamable<type>::rename_logger(const std::string& name)
{
    loggable<type>::rename_logger(name);
    log_stream_.reset(new log_stream(loggable<type>::get_logger(), log_stream_->get_level()));
}

}

#endif
