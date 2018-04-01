/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_FILTER_HPP__)
#define CHUCHO_FILTER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>

namespace chucho
{

/**
 * @class filter filter.hpp chucho/filter.hpp 
 * Conditionally filter an event. A writer may have a set of 
 * filters that provide finer-grained control of log output. 
 * Filter evaluation results in one of three values @ref result
 * "DENY", @ref result "NEUTRAL", or @ref result "ACCEPT". If 
 * the evaluation of all of a writer's filters results in @ref 
 * result "NEUTRAL" or @ref result "ACCEPT", then the event is 
 * allowed to be written. 
 *  
 * @ingroup filters 
 */
class CHUCHO_EXPORT filter : public status_reporter,
                             public configurable
{
public:
    /**
     * The result of filter evaluation.
     */
    enum class result
    {
        /**
         * The event is denied. This value prevents subsequent filters 
         * from being evaluated. It is a vetoing result.
         */
        DENY,
        /**
         * The event may pass. Subsequent filters are evaluated, whose 
         * results may affect the ultimate outcome. 
         */
        NEUTRAL,
        /**
         * The event is accepted. No other filters will be evaluated. 
         * The acceptance is unconditional. 
         */
        ACCEPT
    };

    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Construct a filter.
     *
     * @param name the name of this filter
     */
    filter(const std::string& name);
    /**
     * Destory the filter.
     */
    virtual ~filter() { };

    /**
     * Evaluate the filter. Determine how this filter feels about 
     * this event. 
     * 
     * @param evt the event to evaluate
     * @return result the level of acceptance
     */
    virtual result evaluate(const event& evt) = 0;
    /*
     * Return the name of this filter.
     *
     * @return the name
     */
    const std::string& get_name() const;

private:
    std::string name_;
};

inline filter::filter(const std::string& name)
    : name_(name)
{
}

inline const std::string& filter::get_name() const
{
    return name_;
}

}

#endif
