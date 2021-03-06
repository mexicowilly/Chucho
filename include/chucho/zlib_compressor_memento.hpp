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

#if !defined(CHUCHO_ZLIB_COMPRESSOR_MEMENTO_HPP_)
#define CHUCHO_ZLIB_COMPRESSOR_MEMENTO_HPP_

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class zlib_compressor_memento : public memento
{
public:
    zlib_compressor_memento(configurator& cfg);

    const optional<int>& get_compression_level() const;

private:
    optional<int> compression_level_;
};

inline const optional<int>& zlib_compressor_memento::get_compression_level() const
{
    return compression_level_;
}

}

#endif
