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

#include <chucho/file_writer.hpp>
#include "error_util.hpp"

namespace chucho
{

void file_writer::open_impl(const std::string& file_name)
{
    DWORD dis;
    if (has_been_opened_ || start_ == on_start::APPEND)
        dis = allow_creation_ ? OPEN_ALWAYS : OPEN_EXISTING;
    else
        dis = allow_creation_ ? CREATE_ALWAYS : TRUNCATE_EXISTING;
    HANDLE hnd = CreateFileA(file_name.c_str(),
                             GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                             NULL,
                             dis,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    if (hnd == INVALID_HANDLE_VALUE)
    {
        report_error("Unable to open " + file_name + " for writing: " + error_util::message(GetLastError()));
        is_open_ = false;
    }
    else
    {
        set_file_handle(hnd);
        is_open_ = true;
        if (has_been_opened_ || start_ == on_start::APPEND)
        {
            LARGE_INTEGER zero;
            zero.QuadPart = 0;
            SetFilePointerEx(hnd, zero, NULL, FILE_END);
        }
    }
}

}

