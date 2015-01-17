/*
 * Copyright 2013-2014 Will Mason
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

#if !defined(CHUCHO_EMITTER_HPP__)
#define CHUCHO_EMITTER_HPP__

#include "properties.hpp"
#include <ostream>
#include <vector>

namespace chucho
{

namespace config_tool
{

class emitter
{
public:
    virtual void emit(std::ostream& stream, std::size_t shifts) = 0;
    virtual bool is_valid() const = 0;

protected:
    emitter(const properties& props);

    const properties& get_properties() const;
    std::ostream& indent(std::ostream& stream, std::size_t shifts);

private:
    const properties& props_;
    std::vector<char> spaces_;
};

inline const properties& emitter::get_properties() const
{
    return props_;
}

}

}

#endif
