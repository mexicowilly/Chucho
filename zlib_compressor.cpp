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
    : z_(new z_stream)
{
    set_status_origin("zlib_compressor");
    if (compression_level != Z_DEFAULT_COMPRESSION && (compression_level < 0 || compression_level > 9))
    {
        z_.release();
        throw std::invalid_argument("Compresssion level must be an integer from 0 to 9");
    }
    std::memset(z_.get(), 0, sizeof(z_stream));
    z_->zalloc = Z_NULL;
    z_->zfree = Z_NULL;
    z_->opaque = Z_NULL;
    int rc = deflateInit(z_.get(), compression_level);
    if (rc != Z_OK)
    {
        z_.release();
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

zlib_compressor::~zlib_compressor()
{
    if (z_)
    {
        try
        {
            deflateEnd(z_.get());
            z_.release();
            report_warning("The compressor is closing without finish() having been called. Some data my be lost.");
        }
        catch (...)
        {
        }

    }
}

void zlib_compressor::compress(const std::vector<std::uint8_t>& in,
                               std::vector<std::uint8_t>& out)
{
    if (z_ && !in.empty())
    {
        z_->avail_in = in.size();
        z_->next_in = const_cast<std::uint8_t*>(&in[0]);
        std::array<std::uint8_t, BUFSIZ> buf;
        while (z_->avail_in > 0)
        {
            z_->next_out = buf.data();
            z_->avail_out = buf.size();
            int rc = deflate(z_.get(), Z_NO_FLUSH);
            if (rc == Z_STREAM_ERROR)
            {
                std::string err_msg("Unknown error");
                if (z_->msg != nullptr)
                    err_msg = z_->msg;
                throw exception("Unable to compress data: " + err_msg);
            }
            out.insert(out.end(), buf.begin(), buf.begin() + (buf.size() - z_->avail_out));
        }
    }
    else if (!z_)
    {
        report_warning("The compressor has been finalized. No further compression can take place.");
    }
}

void zlib_compressor::finish(std::vector<std::uint8_t>& out)
{
    if (z_)
    {
        z_->avail_in = 0;
        z_->next_in = Z_NULL;
        // We have to just wing it here. The function deflatePending() is
        // not giving the right result, so we have no idea how much data
        // might come out. I'm just assuming that 4xBUFSIZ will be enough.
        // BUFSIZ is usually 8K, and I can't imagine that zlib keeps caches
        // bigger than that.
        std::vector<std::uint8_t> buf(BUFSIZ * 4);
        z_->avail_out = buf.size();
        z_->next_out = &buf[0];
        deflate(z_.get(), Z_FINISH);
        out.insert(out.end(), buf.begin(), buf.begin() + (buf.size() - z_->avail_out));
        deflateEnd(z_.get());
        z_.release();
    }
}

}