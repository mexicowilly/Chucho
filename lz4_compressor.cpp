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

#include <chucho/lz4_compressor.hpp>
#include <lz4.h>

namespace chucho
{

lz4_compressor::lz4_compressor()
{
    set_status_origin("lz4_compressor");
}

std::vector<std::uint8_t> lz4_compressor::compress(const std::vector<std::uint8_t>& in)
{
    std::vector<std::uint8_t> result(LZ4_compressBound(in.size()));
    auto sz = LZ4_compress_default(reinterpret_cast<const char*>(&in[0]),
                                   reinterpret_cast<char*>(&result[0]),
                                   in.size(),
                                   result.size());
    result.resize(sz);
    return result;
}

}
