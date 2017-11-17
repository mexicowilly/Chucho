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

#if !defined(CHUCHO_LZMA_COMPRESSOR_HPP__)
#define CHUCHO_LZMA_COMPRESSOR_HPP__

#include <chucho/compressor.hpp>

namespace chucho
{

/**
 * @class lzma_compressor lzma_compressor.hpp chucho/lzma_compressor.hpp
 * A compressor that uses LZMA.
 *
 * @ingroup compressors
 */
class CHUCHO_EXPORT lzma_compressor : public compressor
{
public:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct an LZMA compressor.
     */
    lzma_compressor();
    /** @} */

    virtual std::vector<std::uint8_t> compress(const std::vector<std::uint8_t>& in) override;
};

}

#endif
