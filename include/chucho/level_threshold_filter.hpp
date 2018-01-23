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

#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/filter.hpp>
#include <chucho/level.hpp>

namespace chucho
{

/**
 * @class level_threshold_filter level_threshold_filter.hpp chucho/level_threshold_filter.hpp 
 * A filter that tests the log @ref level. The exact same check 
 * on the @ref level that is performed by a @ref logger when it 
 * is determining whether an event is filtered or not is 
 * performed by this filter. This allows further refinement. A 
 * @ref logger can have one @ref writer filtering at one @ref 
 * level and another @ref writer filtering at another @ref 
 * level.
 * 
 * @ingroup filters
 */
class CHUCHO_EXPORT level_threshold_filter : public filter
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a level_threshold_filter.
     *
     * @param name the name of this filter
     * @param lvl the log level at which to filter
     * @throw std::invalid_argument if the level is an uninitialized 
     *        std::shared_ptr
     */
    level_threshold_filter(const std::string& name,
                           std::shared_ptr<level> lvl);
    //@}

    /**
     * Evaluate the event. If the log level of the event is greater 
     * than the log level of this filter, then return 
     * @ref result "result::NEUTRAL". Otherwise, return 
     * @ref result "result::DENY".
     * 
     * @param evt the event to evaluate
     * @return @ref result "result::NEUTRAL" if the log level of the
     *         event is greater than or equal to the level of this
     *         filter. Otherwise, return @ref result "result::DENY".
     */
    virtual result evaluate(const event& evt) override;
    /**
     * Return the level being used by this filter.
     * 
     * @return the level
     */
    std::shared_ptr<level> get_level() const;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_filter::get_level() const
{
    return level_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
