/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/lzma_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <fstream>
#include <lzma.h>

namespace chucho
{

lzma_file_compressor::lzma_file_compressor(unsigned min_idx)
    : file_compressor(min_idx, ".xz")
{
}

void lzma_file_compressor::compress(const std::string& file_name)
{
    std::ifstream in(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!in.is_open())
        throw exception("Could not open " + file_name + " for reading");
    std::string to_write = file_name + get_extension();
    std::ofstream out(to_write.c_str(), std::ios::out | std::ios::binary);
    if (!out.is_open())
        throw exception("Could not open " + to_write + " for reading");
    lzma_stream lzma = LZMA_STREAM_INIT;
    struct sentry
    {
        sentry(lzma_stream* lp) : lzmap(lp) {}
        ~sentry() { lzma_end(lzmap); }
        lzma_stream* lzmap;
    } sent(&lzma);
    if (lzma_easy_encoder(&lzma, 6, LZMA_CHECK_CRC64) == LZMA_MEM_ERROR)
        throw std::runtime_error("Out of memory");
    lzma_action action = LZMA_RUN;
    std::uint8_t in_buf[BUFSIZ];
    std::uint8_t out_buf[BUFSIZ];
    lzma.next_in = nullptr;
    lzma.avail_in = 0;
    lzma.next_out = out_buf;
    lzma.avail_out = sizeof(out_buf);
    while (true)
    {
        if (lzma.avail_in == 0 && !in.eof())
        {
            in.read(reinterpret_cast<char*>(in_buf), sizeof(in_buf));
            if (in.eof())
                action = LZMA_FINISH;
            else if (!in)
                throw exception("Error reading " + file_name);
            lzma.next_in = in_buf;
            lzma.avail_in = in.gcount();
        }
        lzma_ret rc = lzma_code(&lzma, action);
        if (lzma.avail_out == 0 || rc == LZMA_STREAM_END)
        {
            out.write(reinterpret_cast<char*>(out_buf), sizeof(out_buf) - lzma.avail_out);
            if (!out)
                throw exception("Error writing " + to_write);
            lzma.next_out = out_buf;
            lzma.avail_out = sizeof(out_buf);
        }
        if (rc != LZMA_OK)
        {
            if (rc == LZMA_STREAM_END)
                break;
            else
                throw exception("Error compressing " + file_name);
        }
    }
    file::remove(file_name);
}

}
