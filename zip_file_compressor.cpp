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

#include <chucho/zip_file_compressor.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <archive.h>
#include <archive_entry.h>
#include <fstream>
#include <memory>

namespace chucho
{

zip_file_compressor::zip_file_compressor(unsigned min_idx)
    : file_compressor(min_idx, ".zip")
{
}

void zip_file_compressor::compress(const std::string& file_name)
{
    std::string to_write = file_name + get_extension();
    try
    {
        std::ifstream in(file_name.c_str(), std::ios::in | std::ios::binary);
        if (!in.is_open())
            throw exception("Could not open " + file_name + " for reading");
        std::unique_ptr<struct archive, std::function<void(struct archive*)>>
            arch(archive_write_new(), [] (struct archive* a) { archive_write_free(a); });
        if (!arch)
            throw exception("Could not create a new archive object");
        archive_write_set_format_zip(arch.get());
        if (archive_write_open_filename(arch.get(), to_write.c_str()) != ARCHIVE_OK)
        {
            throw exception(std::string("Could not open ") + to_write+
                            " for writing: " + archive_error_string(arch.get()));
        }
        std::unique_ptr<struct archive_entry, std::function<void(struct archive_entry*)>>
            ent(archive_entry_new2(arch.get()), [] (struct archive_entry* e) { archive_entry_free(e); });
        if (!ent)
            throw exception("Could not create new archive entry object");
        archive_entry_set_pathname(ent.get(), file::base_name(file_name).c_str());
        archive_entry_set_size(ent.get(), file::size(file_name));
        archive_entry_set_filetype(ent.get(), AE_IFREG);
        archive_entry_set_perm(ent.get(), 0644);
        archive_write_header(arch.get(), ent.get());
        char buf[BUFSIZ];
        while (in.good())
        {
            in.read(buf, BUFSIZ);
            if (archive_write_data(arch.get(), buf, in.gcount()) != in.gcount())
            {
                throw exception("Could not write file " + file::base_name(file_name) +
                                " in zip archive " + to_write + ": " + archive_error_string(arch.get()));
            }
        }
        if (!in.eof())
            throw exception("Did not read to end of input file " + file_name + " during zip compression");
        in.close();
        if (archive_write_close(arch.get()) != ARCHIVE_OK)
            throw exception(std::string("Error closing archive: ") + archive_error_string(arch.get()));
        file::remove(file_name);
    }
    catch (...)
    {
        try
        {
            file::remove(to_write);
        }
        catch (...)
        {
            // Don't care
        }
        throw;
    }
}

}
