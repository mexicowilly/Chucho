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

#if !defined(CHUCHO_JSON_FORMATTER_HPP__)
#define CHUCHO_JSON_FORMATTER_HPP__

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
 * @class json_formatter json_formatter.hpp chucho/json_formatter.hpp
 * A formatter that produces JSON.
 *
 * @ingroup formatters
 */
class CHUCHO_EXPORT json_formatter : public formatter
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
    json_formatter(style styl = DEFAULT_STYLE,
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
    json_formatter(field_disposition dis,
                   const std::vector<field>& fields,
                   style styl = DEFAULT_STYLE,
                   time_zone tz = DEFAULT_TIME_ZONE,
                   const std::string& time_format = DEFAULT_TIME_FORMAT);
    /**
     * Destruct a formatter
     */
    virtual ~json_formatter();
    /**
     * @}
     */

    virtual std::string format(const event& evt) override;

private:
    style style_;
    std::bitset<12> fields_;
    std::unique_ptr<calendar::formatter> fmt_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
