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

#if !defined(CHUCHO_RUBY_EVALUATOR_FILTER_HPP__)
#define CHUCHO_RUBY_EVALUATOR_FILTER_HPP__

#include <chucho/evaluator_filter.hpp>
#include <mutex>

namespace chucho
{

/**
 * @class ruby_evaluator_filter ruby_evaluator_filter.hpp chucho/ruby_evaulator_filter.hpp 
 * Filter by evaulating a Ruby expression. The data carried by 
 * the @ref event are available as Ruby global variables, 
 * therefore they must be reference with a prefix of '$'. The 
 * variables that can be referenced are the following. 
 * <table> 
 *     <tr><th>Variable</th><th>Type</th><th>Meaning</th></tr>
 *     <tr><td>logger</td><td>String</td>
 *         <td>The name of the logger</td></tr>
 *     <tr><td>level</td><td>String</td>
 *         <td>The name of the level</td></tr>
 *     <tr><td>message</td><td>String</td>
 *         <td>The formatted message</td></tr>
 *     <tr><td>timestamp</td><td>Time</td>
 *         <td>The time</td></tr>
 *     <tr><td>file_name</td><td>String</td>
 *         <td>The file name</td></tr>
 *     <tr><td>line_number</td><td>Fixnum</td>
 *         <td>The line number</td></tr>
 *     <tr><td>function</td><td>String</td>
 *         <td>The name of the function</td></tr>
 *     <tr><td>marker</td><td>String</td>
 *         <td>The marker or nil if there is no marker</td></tr>
 * </table>
 * The Ruby expression must return a true or false value.
 * 
 * @ingroup filters
 */
class ruby_evaluator_filter : public evaluator_filter
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct an evaluator.
     *
     * @param name the name of this filter
     * @param expression the Ruby code that should be run to 
     *                   evaulate the filtration disposition
     */
    ruby_evaluator_filter(const std::string& name,
                          const std::string& expression);
    /** @}  */

    virtual result evaluate(const event& evt) override;
    /**
     * Return the Ruby expression.
     * 
     * @return the Ruby expression
     */
    const std::string& get_expression() const;

private:
    std::string expression_;
};

inline const std::string& ruby_evaluator_filter::get_expression() const
{
    return expression_;
}

}

#endif
