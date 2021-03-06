/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(CHUCHO_CONFIGURABLE_HPP_)
#define CHUCHO_CONFIGURABLE_HPP_

#include <chucho/export.h>

namespace chucho
{

/**
 * @class configurable configurable.hpp chucho/configurable.hpp 
 * Indicates configurability of an object. A class that inherits 
 * from configurable can be configured via a chucho 
 * configuration file. Such a class will have a corresponding 
 * @ref memento and @ref configurable_factory class that can be 
 * used to perform configuration. Before configuration occurs, 
 * an instance of the corresponding factory class must be added 
 * to the configurator via the 
 * @ref configurator::add_configurable_factory() method. For the 
 * factories associated with built-in chucho classes, this is 
 * handled automatically. 
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT configurable
{
public:
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy this configurable.
     */
    virtual ~configurable();
    //@}
};

}

#endif
