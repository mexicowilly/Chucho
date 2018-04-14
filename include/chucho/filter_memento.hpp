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

#ifndef CHUCHO_FILTER_MEMENTO_HPP__
#define CHUCHO_FILTER_MEMENTO_HPP__

#include <chucho/nameable_memento.hpp>

namespace chucho
{

/**
 * @class filter_memento filter_memento.hpp chucho/filter_memento.hpp
 * A @ref memento that has common functionality for filters. All
 * filters need a name.
 *
 * @ingroup filters
 */
class CHUCHO_EXPORT filter_memento : public nameable_memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a filter_memento.
     *
     * @param cfg the configurator that is perform configuration
     */
    filter_memento(configurator& cfg);
    //@}
};

}

#endif
