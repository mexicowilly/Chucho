/*
 * Copyright 2013-2015 Will Mason
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
#include "error_util.hpp"
#include <vector>
#include <iterator>
#include <io.h>

namespace chucho
{

namespace file
{

const char dir_sep = '\\';

std::string base_name(const std::string& name)
{
    if (name.empty())
        return ".";
    if (name.length() == 2 && name[1] == ':')
        return name + '\\';
    std::size_t start = 0;
    std::string drive;
    if (name.length() > 2 && name[1] == ':')
    {
        start = 2;
        drive = name.substr(0, 2);
    }
    if (name.find_first_not_of("/\\", start) == std::string::npos)
        return drive + '\\';
    std::size_t end = name.find_last_not_of("/\\");
    std::size_t slash = name.find_last_of("/\\", end);
    return name.substr(slash + 1, end - slash);
}

void create_directories(const std::string& name)
{
    if (_access(name.c_str(), 0) != 0)
    {
        // VS2012 does not support initializer lists
        std::vector<std::string> dirs;
        dirs.push_back(name);
        std::string dir = directory_name(name);
        while (dir != "\\" &&
               dir != "." &&
               !(dir.length() == 3 && dir[1] == ':'))
        {
            dirs.push_back(dir);
            dir = directory_name(dir);
        }
        for (auto i = dirs.rbegin(); i != dirs.rend(); i++)
        {
            if (_access((*i).c_str(), 0) != 0)
                create_directory(*i);
        }
    }
}

void create_directory(const std::string& name)
{
    if (!CreateDirectoryA(name.c_str(), nullptr))
    {
        DWORD err = GetLastError();
        throw file_exception("Could not create directory " + name +
            ": " + error_util::message(err));
    }
}

std::string directory_name(const std::string& name)
{
    if (name.empty())
        return ".";
    if (name.length() == 2 && name[1] == ':')
        return name + '\\';
    std::size_t start = 0;
    std::string drive;
    if (name.length() > 2 && name[1] == ':')
    {
        start = 2;
        drive = name.substr(0, 2);
    }
    if (name.find_first_not_of("/\\", start) == std::string::npos)
        return drive + '\\';
    std::size_t end = name.find_last_not_of("/\\");
    std::size_t slash = name.find_last_of("/\\", end);
    if (slash == std::string::npos)
        return ".";
    end = name.find_last_not_of("/\\", slash);
    if (end == std::string::npos || end < start)
        return drive + "\\";
    return name.substr(0, end + 1);
}

bool exists(const std::string& name)
{
    return _access_s(name.c_str(), 0) == 0;
}

writeability get_writeability(const std::string& name)
{
    writeability result = writeability::NON_WRITEABLE;
    int rc = _access_s(name.c_str(), 2);
    if (rc == 0)
        result = writeability::WRITEABLE;
    else if (rc == ENOENT) 
        result = writeability::NON_EXISTENT;
    return result;
}

bool is_fully_qualified(const std::string& name)
{
    return !name.empty() &&
        (name[0] == '/' || name[0] == '\\') ||
        (name.length() > 3 && name[1] == ':' && (name[2] == '/' || name[2] == '\\'));
}

void remove(const std::string& name)
{
    if (exists(name))
    {
        DWORD attrs = GetFileAttributesA(name.c_str());
        if (attrs & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!RemoveDirectoryA(name.c_str()))
                throw file_exception("Unable to remove directory " + name);
        }
        else
        {
            if (!DeleteFileA(name.c_str()))
                throw file_exception("Unable to remove file " + name);
        }
    }
}

void remove_all(const std::string& name)
{
    if (!exists(name))
        return;
    SHFILEOPSTRUCTA op;
    std::memset(&op, 0, sizeof(op));
    op.wFunc = FO_DELETE;
    std::vector<char> dblz;
    std::copy(name.begin(), name.end(), std::back_inserter(dblz));
    dblz.push_back(0);
    dblz.push_back(0);
    op.pFrom = &dblz[0];
    op.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
    if (SHFileOperationA(&op) != 0)
        throw file_exception("Unable to remove directory tree at " + name);
}

std::uintmax_t size(const std::string& name)
{
    WIN32_FILE_ATTRIBUTE_DATA attrs;
    if (!GetFileAttributesExA(name.c_str(), GetFileExInfoStandard, &attrs))
        throw file_exception("Could not get size of " + name);
    ULARGE_INTEGER biggy;
    biggy.LowPart = attrs.nFileSizeLow;
    biggy.HighPart = attrs.nFileSizeHigh;
    return biggy.QuadPart;
}

}

}
