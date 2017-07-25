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

#include <chucho/file.hpp>
#include <chucho/file_exception.hpp>
#include <vector>
#include <array>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
// Needed for realpath, which is not in <cstdlib>
#include <stdlib.h>
#if defined(CHUCHO_NO_FTS)
#include <dirent.h>
#include <cstdint>
#else
#include <fts.h>
#endif

#if defined(CHUCHO_NO_FTS)
namespace
{

void remove_all_impl(const std::string& name)
{
    DIR* d = opendir(name.c_str());
    if (d == nullptr)
        throw chucho::file_exception("Could not open directory " + name + ": " + std::strerror(errno));
    struct sentry
    {
        sentry(DIR* d) : d_(d) { }
        ~sentry() { closedir(d_); }
        DIR* d_;
    } s(d);
#if defined(CHUCHO_DIRENT_NEEDS_NAME)
    std::vector<std::uint8_t> dirent_bytes(sizeof(struct dirent) +
        pathconf(name.c_str(), _PC_NAME_MAX));
    struct dirent* entry = reinterpret_cast<struct dirent*>(&dirent_bytes[0]);
#else
    struct dirent dirent_bytes;
    struct dirent* entry = &dirent_bytes;
#endif
    struct dirent* result;
    struct stat stat_buf;
    while (true)
    {
        if (readdir_r(d, entry, &result) != 0)
        {
            // error
        }
        if (result == nullptr)
            break;
        if (std::strcmp(entry->d_name, ".") == 0 ||
            std::strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        std::string child(name + '/' + entry->d_name);
        if (stat(child.c_str(), &stat_buf) != 0)
        {
            // error
        }
        if (S_ISDIR(stat_buf.st_mode))
        {
            remove_all_impl(child);
        }
        else
        {
            chucho::file::remove(child);
        }
    }
    chucho::file::remove(name);
}

}
#endif

namespace chucho
{

namespace file
{

const char dir_sep = '/';

std::string base_name(const std::string& name)
{
    if (name.empty())
        return ".";
    // clang has a bug where it returns past the end instead of npos
    if (name.find_first_not_of('/') >= name.length())
        return "/";
    std::size_t end = name.find_last_not_of('/');
    std::size_t slash = name.rfind('/', end);
    return name.substr(slash + 1, end - slash);
}

void create_directories(const std::string& name)
{
    if (access(name.c_str(), F_OK) != 0)
    {
        std::vector<std::string> dirs = { name };
        std::string dir = directory_name(name);
        while (dir != "/" && dir != ".")
        {
            dirs.push_back(dir);
            dir = directory_name(dir);
        }
        for (auto itor = dirs.rbegin(); itor != dirs.rend(); itor++)
        {
            if (access((*itor).c_str(), F_OK) != 0)
                create_directory(*itor);
        }
    }
}

void create_directory(const std::string& name)
{
    if (mkdir(name.c_str(), 0755) != 0)
        throw file_exception("Could not create directory " + name + ": " + std::string(std::strerror(errno)));
}

std::string directory_name(const std::string& name)
{
    // clang has a bug where it returns past the end instead of npos
    if (name.empty())
        return ".";
    if (name.find_first_not_of('/') >= name.length())
        return "/";
    std::size_t end = name.find_last_not_of('/');
    std::size_t slash = name.rfind('/', end);
    if (slash == std::string::npos)
        return ".";
    end = name.find_last_not_of('/', slash);
    // clang has a bug where it returns past the end instead of npos
    if (end >= name.length())
        return "/";
    return name.substr(0, end + 1);
}

bool exists(const std::string& name)
{
    return access(name.c_str(), F_OK) == 0;
}

writeability get_writeability(const std::string& name)
{
    writeability result = writeability::NON_WRITEABLE;
    int rc = access(name.c_str(), W_OK);
    if (rc == 0)
        result = writeability::WRITEABLE;
    else if (errno == ENOENT) 
        result = writeability::NON_EXISTENT;
    return result;
}

bool is_fully_qualified(const std::string& name)
{
    return !name.empty() && name[0] == '/';
}

void remove(const std::string& name)
{
    if (access(name.c_str(), F_OK) == 0 && std::remove(name.c_str()) != 0)
        throw file_exception("Unable to remove " + name + ": " + std::strerror(errno));
}

void remove_all(const std::string& name)
{
    if (access(name.c_str(), F_OK) != 0)
        return;
    struct stat stat_buf;
    if (stat(name.c_str(), &stat_buf) != 0)
    {
        int this_err = errno;
        throw file_exception("Could not stat\"" + name + "\": " + std::strerror(this_err));
    }
    if (!S_ISDIR(stat_buf.st_mode))
    {
        if (std::remove(name.c_str()) != 0)
        {
            int this_err = errno;
            throw file_exception("Could not remove \"" + name + "\"" + std::strerror(this_err));
        }
        return;
    }
#if defined(CHUCHO_NO_FTS)
    remove_all_impl(name);
#else
    const char* names[] = { name.c_str(), nullptr };
    FTS* fts = fts_open(const_cast<char* const*>(names), FTS_NOSTAT, nullptr);
    if (fts == nullptr)
        throw file_exception("Could not open directory hierarchy of \"" + name + "\" for reading");
    struct sentry
    {
        sentry(FTS* f) { fts_ = f; }
        ~sentry() { fts_close(fts_); }
        FTS* fts_;
    } s(fts);
    FTSENT* ent = fts_read(fts);
    std::array<char, PATH_MAX + 1> path_buf;
    while (ent != nullptr)
    {
        if (ent->fts_info & (FTS_DP | FTS_F | FTS_SL))
        {
            if (std::remove(ent->fts_accpath) != 0)
            {
                int this_err = errno;
                realpath(ent->fts_accpath, path_buf.data());
                throw file_exception(std::string("Could not remove \"") + path_buf.data() + "\": " + std::strerror(this_err));
            }
        }
        ent = fts_read(fts);
    }
#endif
}

std::uintmax_t size(const std::string& name)
{
    struct stat info;
    if (stat(name.c_str(), &info) != 0)
        throw file_exception("Could not get size of " + name);
    return info.st_size;
}

}

}
