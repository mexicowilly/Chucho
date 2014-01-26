/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/zip_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <chucho/calendar.hpp>
#include <chucho/version.hpp>
#include <zip.h>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <cstring>

namespace chucho
{

zip_file_compressor::zip_file_compressor(unsigned min_idx)
    : file_compressor(min_idx, ".zip")
{
}

void zip_file_compressor::compress(const std::string& file_name)
{
    std::ifstream in(file_name.c_str(), std::ios::in | std::ios::binary);
    if (!in.is_open())
        throw exception("Could not open " + file_name + " for reading");
    std::string to_write = file_name + get_extension();
    zipFile z = zipOpen(to_write.c_str(), APPEND_STATUS_CREATE);
    if (z == nullptr)
        throw exception("Could not create zip archive " + to_write);
    struct std::tm now = calendar::get_utc(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    zip_fileinfo zipfi;
    std::memset(&zipfi, 0, sizeof(zipfi));
    zipfi.tmz_date.tm_sec = now.tm_sec;
    zipfi.tmz_date.tm_min = now.tm_min;
    zipfi.tmz_date.tm_hour = now.tm_hour;
    zipfi.tmz_date.tm_mday = now.tm_mday;
    zipfi.tmz_date.tm_mon = now.tm_mon;
    zipfi.tmz_date.tm_year = now.tm_year;
    std::string created_msg("Created by Chucho version ");
    created_msg += version::text();
    if (zipOpenNewFileInZip(z,
                            file::base_name(file_name).c_str(),
                            &zipfi,
                            nullptr,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            Z_DEFLATED,
                            Z_DEFAULT_COMPRESSION) != ZIP_OK)
    {
        zipClose(z, created_msg.c_str());
        try
        {
            file::remove(to_write);
        }
        catch (...)
        {
            // don't care
        }
        throw exception("Unable to create file " + file::base_name(file_name) + " in zip archive " + to_write);
    }
    struct sentry
    {
        sentry(zipFile z, const std::string& msg) : z_(z) { }
        ~sentry() { zipCloseFileInZip(z_); zipClose(z_, msg_.c_str()); }
        zipFile z_;
        std::string msg_;
    } s(z, created_msg);
    char buf[BUFSIZ];
    while (in.good())
    {
        in.read(buf, BUFSIZ);
        if (zipWriteInFileInZip(z, buf, static_cast<unsigned>(in.gcount())) != ZIP_OK)
            throw exception("Could not write file " + file::base_name(file_name) + " in zip archive " + to_write);

    }
    if (!in.eof())
        throw exception("Did not read to end of input file " + file_name + " during zip compression");
    in.close();
    file::remove(file_name);
}

}
