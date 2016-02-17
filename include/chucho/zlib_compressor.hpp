/*
 * Copyright 2013-2016 Will Mason
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

class CHUCHO_EXPORT zlib_compressor : public compressor
{
public:
    zlib_compressor(int compression_level = Z_DEFAULT_COMPRESSION);
    virtual ~zlib_compressor();

    virtual void compress(const std::vector<std::uint8_t>& in,
                          std::vector<std::uint8_t>& out) override;
    virtual void finish(std::vector<std::uint8_t>& out) override;

private:
    std::unique_ptr<z_stream> z_;
};

}

#endif