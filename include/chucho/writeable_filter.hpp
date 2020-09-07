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

#if !defined(CHUCHO_WRITEABLE_FILTER_HPP__)
#define CHUCHO_WRITEABLE_FILTER_HPP__

#include <chucho/filter.hpp>
#include <chucho/writer.hpp>

namespace chucho
{

/**
 * @class writeable_filter writeable_filter.hpp chucho/writeable_filter.hpp
 * A filter that is able to write messages to a writer. If you create a
 * writeable_filter by hand, you must either use the constructor that takes
 * a writer or call the @c set_writer method before you use the filter.
 *
 * If, however, you create the filter in a Chucho configuration file, the
 * writer will be set automatically.
 *
 * @ingroup filters
 */
class CHUCHO_EXPORT writeable_filter : public filter
{
public:
    /**
     * Retrieve the writer.
     *
     * @return the writer
     */
    writer& get_writer() const;
    /**
     * Return wether this filter has a writer set or not.
     *
     * @return true if there is a writer, false otherwise
     */
    bool has_writer() const;
    /**
     * Set the writer that this filter will use.
     *
     * @param wrt the writer
     */
    void set_writer(writer& wrt);

protected:
    /**
     * Construct a filter.
     *
     * @param name the name of the filter
     */
    writeable_filter(const std::string& name);
    /**
     * Construct a filter.
     *
     * @param name the name of the filter
     * @param wrt the writer
     */
    writeable_filter(const std::string& name, writer& wrt);

    /**
     * Write an event to the writer.
     *
     * @param evt the event to write
     */
    void write(const event& evt);

private:
    // This is just a settable reference
    writer* writer_;
};

inline writeable_filter::writeable_filter(const std::string&name)
    : filter(name),
      writer_(nullptr)
{
}

inline writeable_filter::writeable_filter(const std::string&name, writer& wrt)
    : filter(name),
      writer_(&wrt)
{
}

inline writer& writeable_filter::get_writer() const
{
    return *writer_;
}

inline bool writeable_filter::has_writer() const
{
    return writer_ != nullptr;
}

inline void writeable_filter::set_writer(writer& wrt)
{
    writer_ = &wrt;
}

inline void writeable_filter::write(const event& evt)
{
    if (has_writer())
        writer_->write_impl(evt);
}

}

#endif