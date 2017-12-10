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

#if !defined(CHUCHO_WRITER_HPP__)
#define CHUCHO_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/filter.hpp>
#include <chucho/formatter.hpp>
#include <chucho/configurable.hpp>
#include <chucho/non_copyable.hpp>
#include <list>
#include <mutex>
#include <vector>

namespace chucho
{

/**
 * @class writer writer.hpp chucho/writer.hpp
 * A writer writes events to whatever destination it is designed 
 * to use. A @ref logger can have as many writers attached to it
 * as it needs. Writers also provide a further means of 
 * filtering events, as each writer has a collection of 
 * @ref filter "filters". The filters are visited in the order 
 * in which they are added.
 *  
 * @ingroup writers 
 */
class CHUCHO_EXPORT writer : non_copyable,
                             public status_reporter,
                             public configurable
{
public:
    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Construct a writer with the given formatter. All events will
     * be formatted using the formatter before being written. 
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::unique_ptr
     */
    writer(const std::string& name, std::unique_ptr<formatter>&& fmt);
    /**
     * Destroy a writer.
     */
    virtual ~writer();
    //@}

    /**
     * Add a filter. Filters are visited in the order in which they 
     * are added. 
     * 
     * @param flt the filter
     */
    void add_filter(std::unique_ptr<filter>&& flt);
    /**
     * Remove all filters.
     */
    void clear_filters();
    /**
     * Flush any buffers or caches.
     */
    virtual void flush();
    /**
     * Receive the collection of all filters.
     * @return the filters
     */
    std::vector<std::string> get_filter_names();
    /**
     * Return this writer's formatter.
     * 
     * @return the formatter
     */
    formatter& get_formatter() const;
    /**
     * Return this writer's name.
     *
     * @return the name
     */
    const std::string& get_name() const;
    /**
     * Remove a named filter.
     *
     * @param name the name of the filter to remove
     */
    void remove_filter(const std::string& name);
    /**
     * Write an event. This non-virtual method takes care of all the 
     * common housekeeping that writers must undertake when writing 
     * an event. A check is made to see whether the current thread 
     * is already writing something, the presence of the formatter 
     * is verified, and the filters are evaluated. If the event can 
     * actually be written, then the virtual method @ref write_impl 
     * is called. 
     * 
     * @param evt the event to write
     */
    void write(const event& evt);

protected:
    /**
     * Write the event. This virtual method is meant to handle the 
     * mechanics of actually writing to the destination. The calling 
     * @ref write method has already taken care of the housekeeping.
     * 
     * @param evt the event to write
     */
    virtual void write_impl(const event& evt) = 0;

    /**
     * The formatter used to turn events into text.
     */
    std::unique_ptr<formatter> formatter_;

private:
    /**
     * @pre guard_ must be locked
     * @param evt the event to evaluate
     * @return true if this writer can write the event
     */
    CHUCHO_NO_EXPORT bool permits(const event& evt);

    std::list<std::unique_ptr<filter>> filters_;
    std::unique_ptr<std::recursive_mutex> guard_;
    bool i_am_writing_;
    std::string name_;
};

inline formatter& writer::get_formatter() const
{
    return *formatter_;
}

inline const std::string& writer::get_name() const {
    return name_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
