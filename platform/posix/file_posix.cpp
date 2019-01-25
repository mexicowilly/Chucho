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

#include <chucho/file.hpp>
#include <chucho/file_exception.hpp>
#include <chucho/environment.hpp>
#include <vector>
#include <array>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
// Needed for realpath, which is not in <cstdlib>
#include <stdlib.h>
#include <dirent.h>
#include <cstdint>

namespace
{

void remove_all_impl(const std::string& name)
{
    chucho::file::directory_iterator itor(name);
    chucho::file::directory_iterator end;
    struct stat st;
    while (itor != end)
    {
        auto& cur = *itor;
        if (stat(cur.c_str(), &st) != 0)
        {
        }
        if (S_ISDIR(st.st_mode))
            remove_all_impl(cur);
        chucho::file::remove(cur);
        ++itor;
    }
    chucho::file::remove(name);
}

}

namespace chucho
{

namespace file
{

const char dir_sep = '/';

struct directory_iterator_impl
{
    directory_iterator_impl(const std::string& dir);
    ~directory_iterator_impl();

    DIR* dir_;
    std::unique_ptr<std::uint8_t[]> dirent_bytes_;
    struct dirent* entry_;
    std::string parent_;
};

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

std::string home_directory()
{
    auto sz = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (sz == -1)
        sz = 8 * 1024;
    struct passwd pw;
    auto pwp = &pw;
    std::vector<char> buf;
    int rc;
    do
    {
        buf.resize(sz);
        rc = getpwuid_r(geteuid(), &pw, &buf[0], buf.size(), &pwp);
        sz *= 2;
    } while (rc == ERANGE);
    std::string result;
    if (pwp != nullptr)
    {
        result = pw.pw_dir;
        if (!result.empty() && result.back() != '/')
            result += '/';
    }
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
    remove_all_impl(name);
}

std::uintmax_t size(const std::string& name)
{
    struct stat info;
    if (stat(name.c_str(), &info) != 0)
        throw file_exception("Could not get size of " + name);
    return info.st_size;
}

std::string temporary_directory()
{
    auto env = environment::get("TMPDIR");
    std::string result = env ? *env : P_tmpdir;
    if (result.empty() || result[result.length() - 1] != '/')
        result += '/';
    return result;
}

directory_iterator::directory_iterator()
{
}

directory_iterator::~directory_iterator()
{
}

directory_iterator::directory_iterator(const std::string& directory)
    : pimpl_(std::make_unique<directory_iterator_impl>(directory))
{
    operator++();
}

bool directory_iterator::operator== (const directory_iterator& it) const
{
    return (!pimpl_ && !it.pimpl_) || (pimpl_ && it.pimpl_ && pimpl_->parent_ == it.pimpl_->parent_ && cur_ == it.cur_);
}

directory_iterator& directory_iterator::operator++ ()
{
    if (pimpl_)
    {
        while (true)
        {
            struct dirent* result;
            if (readdir_r(pimpl_->dir_, pimpl_->entry_, &result) != 0)
                throw chucho::file_exception("Could not read directory " + pimpl_->parent_ + ": " + std::strerror(errno));
            if (result == nullptr)
            {
                pimpl_.reset();
                cur_.clear();
                break;
            }
            else
            {
                if (std::strcmp(result->d_name, ".") != 0 && std::strcmp(result->d_name, "..") != 0)
                {
                    cur_ = pimpl_->parent_ + result->d_name;
                    break;
                }
            }
        };
    }
    return *this;
}

directory_iterator_impl::directory_iterator_impl(const std::string& dir)
    : dir_(opendir(dir.c_str())),
      parent_(dir)
{
    if (dir_ == nullptr)
        throw chucho::file_exception("Could not open directory " + dir + ": " + std::strerror(errno));
    if (parent_[parent_.length() - 1] != '/')
        parent_ += '/';
#if defined(CHUCHO_DIRENT_NEEDS_NAME)
    dirent_bytes_ = std::make_unique<std::uint8_t[]>(sizeof(struct dirent) + pathconf(dir.c_str(), _PC_NAME_MAX));
#else
    dirent_bytes_ = std::make_unique<std::uint8_t[]>(sizeof(struct dirent));
#endif
    entry_ = reinterpret_cast<struct dirent*>(dirent_bytes_.get());
}

directory_iterator_impl::~directory_iterator_impl()
{
    closedir(dir_);
}

}

}
