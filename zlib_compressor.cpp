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

#include <chucho/zlib_compressor.hpp>
#include <chucho/exception.hpp>
#include <stdexcept>
#include <sstream>
#include <cstdio>
#include <array>
#include <cstring>

namespace chucho
{

zlib_compressor::zlib_compressor(int compression_level)
    : compression_level_(compression_level)
{
    set_status_origin("zlib_compressor");
    if (compression_level != Z_DEFAULT_COMPRESSION && (compression_level < 0 || compression_level > 9))
        throw std::invalid_argument("Compresssion level must be an integer from 0 to 9");
    z_stream z;
    std::memset(&z, 0, sizeof(z));
    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    int rc = deflateInit(&z, compression_level);
    deflateEnd(&z);
    if (rc != Z_OK)
    {
        if (rc == Z_MEM_ERROR)
        {
            throw std::runtime_error("Out of memory");
        }
        else if (rc == Z_VERSION_ERROR)
        {
            std::ostringstream stream;
            stream << "The version of zlib with which Chucho was built was " << ZLIB_VERSION << ", but the runtime version is the incompatible " << zlibVersion();
            throw exception(stream.str());
        }
        // No other cases are documented to occur, because we've already checked
        // the compression level
    }
}

std::vector<std::uint8_t> zlib_compressor::compress(const std::vector<std::uint8_t>& in)
{
    std::vector<std::uint8_t> out;
    if (!in.empty())
    {
        z_stream z;
        std::memset(&z, 0, sizeof(z));
        z.zalloc = Z_NULL;
        z.zfree = Z_NULL;
        z.opaque = Z_NULL;
        deflateInit(&z, compression_level_);
        struct sentry
        {
            sentry(z_stream* zp) : zp_(zp) { }
            ~sentry() { deflateEnd(zp_); }
            z_stream* zp_;
        } sent(&z);
        z.avail_in = in.size();
        z.next_in = const_cast<std::uint8_t*>(&in[0]);
        std::array<std::uint8_t, BUFSIZ> buf;
        while (z.avail_in > 0)
        {
            z.next_out = buf.data();
            z.avail_out = buf.size();
            int rc = deflate(&z, Z_NO_FLUSH);
            if (rc == Z_STREAM_ERROR)
            {
                std::string err_msg("Unknown error");
                if (z.msg != nullptr)
                    err_msg = z.msg;
                throw exception("Unable to compress data: " + err_msg);
            }
            out.insert(out.end(), buf.begin(), buf.begin() + (buf.size() - z.avail_out));
        }
        z.avail_in = 0;
        z.next_in = Z_NULL;
        int rc = Z_OK;
        while (rc != Z_STREAM_END)
        {
            z.avail_out = buf.size();
            z.next_out = buf.data();
            rc = deflate(&z, Z_FINISH);
            if (rc != Z_OK && rc != Z_STREAM_END && rc != Z_BUF_ERROR)
            {
                report_error("Unable to complete stream compression");
                std::string err_msg("Unknown error");
                if (z.msg != nullptr)
                    err_msg = z.msg;
                throw exception("Unable to complete compression: " + err_msg);
            }
            out.insert(out.end(), buf.begin(), buf.begin() + (buf.size() - z.avail_out));
        }
    }
    return out;
}

}
