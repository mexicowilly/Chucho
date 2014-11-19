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

#if !defined(CHUCHO_PROPERTIES_HPP__)
#define CHUCHO_PROPERTIES_HPP__

#include <cstddef>

namespace chucho
{

namespace config_tool
{

class properties
{
public:
    properties();

    std::size_t get_shift_width() const;
    void set_shift_width(std::size_t sw);

private:
    std::size_t shift_width_;
};

inline std::size_t properties::get_shift_width() const
{
    return shift_width_;
}

inline void properties::set_shift_width(std::size_t sw)
{
    shift_width_ = sw;
}

}

}

#endif
