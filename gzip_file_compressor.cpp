/*
 * Copyright 2013-2019 Will Mason
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

#include <chucho/gzip_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <zlib.h>
#include <fstream>
#include <cstdio>
#include <cstring>

namespace chucho
{

gzip_file_compressor::gzip_file_compressor(unsigned min_idx)
    : file_compressor(min_idx, ".gz")
{
}

void gzip_file_compressor::compress(const std::string& file_name)
{
    std::ifstream in(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!in.is_open())
        throw exception("Could not open " + file_name + " for reading");
    std::string to_write = file_name + get_extension();
    gzFile gz = gzopen(to_write.c_str(), "w");
    if (gz == nullptr)
        throw exception("Could not open " + to_write + " for writing");
    struct sentry
    {
        sentry(gzFile gz) : gz_(gz) { }
        ~sentry() { gzclose(gz_); }
        gzFile gz_;
    } s(gz);
    char buf[BUFSIZ];
    while (in.good())
    {
        in.read(buf, BUFSIZ);
        if (gzwrite(gz, buf, static_cast<unsigned>(in.gcount())) <= 0)
        {
            int err;
            const char* err_msg = gzerror(gz, &err);
            if (err != Z_OK) 
            {
                if (err == Z_ERRNO) 
                    err_msg = std::strerror(errno);
                throw exception("Could not write to " + to_write + ": (" + std::to_string(err) + ") " + std::string(err_msg));
            }
        }
    }
    if (!in.eof())
        throw exception("Did not read to end of input file " + file_name + " during gzip compression");
    in.close();
    file::remove(file_name);
}

}
