/*
 * Copyright 2013 Will Mason
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

#include <chucho/bzip2_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <bzlib.h>
#include <fstream>
#include <cstdio>

namespace chucho
{

bzip2_file_compressor::bzip2_file_compressor(unsigned min_idx)
    : file_compressor(min_idx, ".bz2")
{
}

void bzip2_file_compressor::compress(const std::string& file_name)
{
    std::ifstream in(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!in.is_open())
        throw exception("Could not open " + file_name + " for reading");
    std::string to_write = file_name + get_extension();
    FILE* f = std::fopen(to_write.c_str(), "w");
    if (f == nullptr)
        throw exception("Could not open " + to_write + " for writing");
    int err;
    BZFILE* bz = BZ2_bzWriteOpen(&err, f, 9, 0, 0);
    struct sentry
    {
        sentry(FILE* f, BZFILE* bz) : f_(f), bz_(bz) { }
        ~sentry() { int err; BZ2_bzWriteClose(&err, bz_, 0, nullptr, nullptr); std::fclose(f_); }
        FILE* f_;
        BZFILE* bz_;
    } s(f, bz);
    if (err != BZ_OK)
        throw exception("Could not initialize " + to_write + " for bzip2 compression");
    char buf[BUFSIZ];
    while (in.good())
    {
        in.read(buf, BUFSIZ);
        BZ2_bzWrite(&err, bz, buf, in.gcount());
        if (err != BZ_OK)
            throw exception("Could not write to bzip2 compressed file " + to_write);
    }
    if (!in.eof())
        throw exception("Did not read to end of input file during gzip compression");
    file::remove(file_name);
}

}

