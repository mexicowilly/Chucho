/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_EVALUATOR_FILTER_HPP__)
#define CHUCHO_EVALUATOR_FILTER_HPP__

#include <chucho/filter.hpp>

namespace chucho
{

/**
 * @class evaluator_filter evaluator_filter.hpp chucho/evaluator_filter.hpp 
 * An evaluator filter determines the filtration status of an 
 * @ref event by evaluating a piece of code. The code that is 
 * run has access to all the data carried in the @ref event, so 
 * that it can evaluate it with full knowledge. 
 *  
 * @ingroup filters 
 * @sa ruby_evaluator_filter 
 */

class evaluator_filter : public filter
{
};

}

#endif
