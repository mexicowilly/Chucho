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

#include <chucho/bzip2_compressor.hpp>
#include <chucho/exception.hpp>
#include <bzlib.h>
#include <math.h>

namespace chucho
{

bzip2_compressor::bzip2_compressor()
{
    set_status_origin("bzip2_compressor");
}

std::vector<std::uint8_t> bzip2_compressor::compress(const std::vector<std::uint8_t>& in)
{
    unsigned count = ceil(static_cast<double>(in.size()) * 1.05) + 600.0;
    std::vector<std::uint8_t> result(count);
    int rc = BZ2_bzBuffToBuffCompress(reinterpret_cast<char*>(&result[0]),
                                      &count,
                                      const_cast<char*>(reinterpret_cast<const char*>(&in[0])),
                                      in.size(),
                                      9,
                                      0,
                                      0);
    while (rc == BZ_OUTBUFF_FULL)
    {
        report_info("Resizing the output buffer from " + std::to_string(count) + " to " + std::to_string(count + in.size()));
        count = count + in.size();
        result.resize(count);
        rc = BZ2_bzBuffToBuffCompress(reinterpret_cast<char*>(&result[0]),
                                      &count,
                                      const_cast<char*>(reinterpret_cast<const char*>(&in[0])),
                                      in.size(),
                                      9,
                                      0,
                                      0);

    }
    if (rc != BZ_OK)
        throw exception("Insufficient memory to perform bzip2 compression");
    result.resize(count);
    return result;
}

}
