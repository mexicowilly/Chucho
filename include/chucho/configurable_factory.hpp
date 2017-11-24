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

#if !defined(CHUCHO_CONFIGURABLE_FACTORY_HPP__)
#define CHUCHO_CONFIGURABLE_FACTORY_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/memento.hpp>
#include <chucho/configurable.hpp>
#include <chucho/configurator.hpp>
#include <chucho/status_reporter.hpp>
#include <memory>

namespace chucho
{

/**
 * @class configurable_factory configurable_factory.hpp chucho/configurable_factory.hpp 
 * A factory for configurable classes. Each class that expects 
 * to be instantiated during configuration time must have both a
 * @ref configurable_factory and a @ref memento associated with 
 * it. The factory will be asked to create first the memento and
 * then the configurable object itself using the memento as 
 * input. 
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT configurable_factory : public status_reporter
{
public:
    /**
     * Construct a configurable_factory.
     */
    configurable_factory();
    /**
     * Destroy a configurable_factory.
     */
    virtual ~configurable_factory();

    /**
     * Create an instance of a @ref configurable. The 
     * @ref configurable to which this factory corresponds is 
     * created using the memento that was previously created by this
     * factory. 
     * 
     * @param mnto the memento
     * @return the configurable
     */
    virtual std::unique_ptr<configurable> create_configurable(const memento& mnto) = 0;
    /**
     * Create an instance of a memento. The memento is used by the 
     * @ref configurator to gather enough information to create the 
     * @ref configurable after enough information about the @ref 
     * configurable has been gathered at configuration time.
     * 
     * @param cfg the configurator currently in action
     * @return the memento
     */
    virtual std::unique_ptr<memento> create_memento(configurator& cfg) = 0;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
