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

#if !defined(CHUCHO_LEVEL_FILTER_HPP__)
#define CHUCHO_LEVEL_FILTER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/filter.hpp>

namespace chucho
{

/**
 * @class level_filter level_filter.hpp chucho/level_filter.hpp 
 * A filter that targets a specific log level. Unlike the 
 * level_threshold_filter, which functions just as the primary 
 * filtering mechanism in @ref logger functions, this @ref 
 * filter targets a specific @ref level. A matching and a 
 * non-matching @ref result are accepted in the constructor. 
 * 
 * @ingroup filters
 */
class CHUCHO_EXPORT level_filter : public filter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a level_filter.
     * 
     * @param lvl the log level to target
     * @param on_match the result if the event's level matches
     * @param on_mismatch the result if the event's level does not 
     *                    match
     */
    level_filter(std::shared_ptr<level> lvl,
                 result on_match,
                 result on_mismatch);
    //@}

    /**
     * Return the matching result of this filter if the event's 
     * level matches this filter's level. Otherwise, return the 
     * non-matching result. 
     * 
     * @param evt the event to test
     * @return either the matching or the non-matching result
     */
    virtual result evaluate(const event& evt) override;
    /**
     * Return the log level that this filter is targeting.
     * 
     * @return the level
     */
    std::shared_ptr<level> get_level() const;
    /**
     * Return the result when the event's @ref level matches the 
     * level of this filter. 
     * 
     * @return the matching result
     */
    result get_on_match() const;
    /**
     * Return the result when the event's @ref level does not match 
     * the level of this filter. 
     * 
     * @return the non-matching result
     */
    result get_on_mismatch() const;

private:
    std::shared_ptr<level> level_;
    result on_match_;
    result on_mismatch_;
};

inline std::shared_ptr<level> level_filter::get_level() const
{
    return level_;
}

inline filter::result level_filter::get_on_match() const
{
    return on_match_;
}

inline filter::result level_filter::get_on_mismatch() const
{
    return on_mismatch_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
