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

#if !defined(CHUCHO_WRITER_FACTORY_HPP_)
#define CHUCHO_WRITER_FACTORY_HPP_

#include <chucho/configurable_factory.hpp>
#include <chucho/writer_memento.hpp>

namespace chucho
{

/**
 * @class writer_factory writer_factory.hpp chucho/writer_factory.hpp
 * A @ref configurable_factory that has common functionality for 
 * writers. All writers can have filters that are discovered at 
 * configuration time. This class provides a helper method to 
 * facilitate setting a writer's filters based on its memento. 
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT writer_factory : public configurable_factory
{
protected:
    /**
     * Set a writer's filters. The @ref configurable here must be a 
     * @ref writer. It's filters are extracted from the @ref memento 
     * and set appropriately.
     * 
     * @param cnf the @ref writer that wants filters
     * @param mnto the @ref memento that has the filters
     */
    void set_filters(configurable& cnf, writer_memento& mnto);
};

}

#endif
