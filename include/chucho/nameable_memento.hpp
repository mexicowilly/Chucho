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

#ifndef CHUCHO_NAMEABLE_MEMENTO_HPP_
#define CHUCHO_NAMEABLE_MEMENTO_HPP_

#include <chucho/memento.hpp>
#include <typeinfo>

namespace chucho
{

/**
 * @class nameable_memento nameable_memento.hpp chucho/nameable_memento.hpp
 * A @ref memento that has common functionality for things that
 * get named. These include writers and filters.
 *  
 * @ingroup configuration 
 */
class nameable_memento : public memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a memento.
     *
     * @param cfg the configurator that is perform configuration
     */
    nameable_memento(configurator& cfg);
    //@}
    /**
     * Return the name of the memento.
     *
     * @return the name
     */
    const std::string& get_name() const;

    /**
     * Set the default name of the object to be created.
     *
     * @param ti the type
     */
    void set_default_name(const std::type_info& ti);

private:
    std::string name_;
};

inline const std::string& nameable_memento::get_name() const
{
    return name_;
}

}

#endif
