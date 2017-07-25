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

#if !defined(CHUCHO_ZLIB_COMPRESSOR_HPP__)
#define CHUCHO_ZLIB_COMPRESSOR_HPP__

#include <chucho/compressor.hpp>
#include <zlib.h>

namespace chucho
{

/**
 * @class zlib_compressor zlib_compressor.hpp chucho/zlib_compressor.hpp
 * A compressor that uses zlib.
 * 
 * @ingroup compressors
 */
class CHUCHO_EXPORT zlib_compressor : public compressor
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a zlib compressor with the given compression level.
     * Valid compression levels are [0, 9].
     * 
     * @param compression_level the level, which can be [0, 9]
     */
    zlib_compressor(int compression_level = Z_DEFAULT_COMPRESSION);
    /**
     * @}
     */

    virtual std::vector<std::uint8_t> compress(const std::vector<std::uint8_t>& in) override;

private:
    int compression_level_;
};

}

#endif
