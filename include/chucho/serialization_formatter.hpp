/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_SERIALIZATION_FORMATTER_HPP__)
#define CHUCHO_SERIALIZATION_FORMATTER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/formatter.hpp>
#include <bitset>
#include <vector>

namespace chucho
{

namespace calendar
{

class formatter;

}

/**
 * @class serialization_formatter serialization_formatter.hpp chucho/serialization_formatter.hpp
 * A formatter that produces output in a serialized fashion. Rather than formatting an event
 * in such a way as to produce a single message, like @ref pattern_formatter, this
 * formatter will include selected event fields in a serialized manner.
 *
 * @sa json_formatter, yaml_formatter
 *
 * @ingroup formatters
 */
class CHUCHO_EXPORT serialization_formatter : public formatter
{
public:
    /**
     * The style of the output.
     */
    enum class style
    {
        PRETTY,     /**< Human readable output */
        COMPACT     /**< Machine readable output */
    };

    /**
     * Fields that can appear in the output.
     */
    enum class field
    {
        DIAGNOSTIC_CONTEXT,     /**< The thread's @ref diagnostic_context */
        FILE_NAME,              /**< The file name of the event */
        FUNCTION_NAME,          /**< The function name */
        HOST_NAME,              /**< The host on which the event occurred */
        LEVEL,                  /**< The level of the event */
        LINE_NUMBER,            /**< The line number */
        LOGGER,                 /**< The name of the logger */
        MARKER,                 /**< The marker if there is one */
        MESSAGE,                /**< The message */
        PROCESS_ID,             /**< The pid */
        THREAD,                 /**< The thread identifier */
        TIMESTAMP               /**< The timestamp, formatted according to the time format parameter */
    };

    /**
     * Whether fields are included or excluded in the constructor
     */
    enum class field_disposition
    {
        INCLUDED,   /**< Fields are included */
        EXCLUDED    /**< Fields are excluded */
    };

    /**
     * The time zone of the timestamp field
     */
    enum class time_zone
    {
        LOCAL,  /**< Local time */
        UTC     /**< UTC */
    };

    /**
     * The default style
     */
    static constexpr style DEFAULT_STYLE = style::COMPACT;
    /**
     * The default time zone
     */
    static constexpr time_zone DEFAULT_TIME_ZONE = time_zone::LOCAL;
    /**
     * The default time format
     */
    static constexpr char const* DEFAULT_TIME_FORMAT = "%Y%m%dT%H%M%S";

protected:
    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct a formatter with all fields included.
     *
     * @param styl the style, pretty or compact
     * @param tz the time zone
     * @param time_format the time format
     */
    serialization_formatter(style styl = DEFAULT_STYLE,
                            time_zone tz = DEFAULT_TIME_ZONE,
                            const std::string& time_format = DEFAULT_TIME_FORMAT);
    /**
     * Construct a formatter with only certain fields included
     * or excluded.
     *
     * @param dis whether the given fields are included or excluded
     * @param fields the fields to include or exclude
     * @param styl the style, pretty or compact
     * @param tz the time zone
     * @param time_format the time format
     */
    serialization_formatter(field_disposition dis,
                            const std::vector<field>& fields,
                            style styl = DEFAULT_STYLE,
                            time_zone tz = DEFAULT_TIME_ZONE,
                            const std::string& time_format = DEFAULT_TIME_FORMAT);
    /**
     * Destroy a formatter.
     */
    ~serialization_formatter();
    /**
     * @}
     */

    style style_;
    std::bitset<12> fields_;
    std::unique_ptr<calendar::formatter> cal_fmt_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
