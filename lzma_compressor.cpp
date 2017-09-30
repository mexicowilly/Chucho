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

#include <chucho/lzma_compressor.hpp>
#include <lzma.h>

namespace chucho
{

lzma_compressor::lzma_compressor()
{
    set_status_origin("lzma_compressor");
}

std::vector<std::uint8_t> lzma_compressor::compress(const std::vector<std::uint8_t>& in)
{
    std::vector<std::uint8_t> result(lzma_stream_buffer_bound(in.size()));
    std::size_t out_pos = 0;
    lzma_easy_buffer_encode(6,
                            LZMA_CHECK_CRC64,
                            nullptr,
                            &in[0],
                            in.size(),
                            &result[0],
                            &out_pos,
                            result.size());
    result.resize(out_pos);
    return result;
}

}

