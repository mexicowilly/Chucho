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

#include <chucho/utf8.hpp>
#include <chucho/exception.hpp>
#include <string>
#include <sstream>
#include <iomanip>

namespace
{


std::string escape(char c)
{
    std::ostringstream stream;
    stream << "\\x" << std::hex << std::setfill('0') <<
        std::setw(2) << (static_cast<unsigned>(c) & 0xff);
    return stream.str();
}

std::string::size_type find_invalid(const std::string& utf8, std::string::size_type pos = 0)
{
    unsigned char ch;
    std::string::size_type i = pos;
    std::string::size_type j;
    std::string::size_type end;
    std::size_t trailing;
    while (i < utf8.length())
    {
        ch = static_cast<unsigned char>(utf8[i]);
        if (ch <= 0x7f)
        {
            i++;
            continue;
        }
        else if (ch >= 0xc0 && ch <= 0xdf)
        {
            trailing = 1;
        }
        else if (ch >= 0xe0 && ch <= 0xef)
        {
            trailing = 2;
        }
        else if (ch >= 0xf0 && ch <= 0xf4)
        {
            trailing = 3;
        }
        else
        {
            return i;
        }
        end = i + trailing;
        if (end >= utf8.length())
            return i;
        for (j = i + 1; j <= end; j++)
        {
            ch = static_cast<unsigned char>(utf8[j]);
            if (ch < 0x80 || ch > 0xbf)
                return i;
        }
        i = j;
    }
    return std::string::npos;
}

}

namespace chucho
{

namespace utf8
{

std::string escape_invalid(const std::string& text)
{
    auto invalid = find_invalid(text);
    if (invalid == std::string::npos)
    {
        return text;
    }
    else
    {
        std::string::size_type prev = 0;
        std::string result;
        while (invalid != std::string::npos)
        {
            result.append(text.substr(prev, invalid - prev));
            result.append(escape(text[invalid]));
            prev = invalid + 1;
            invalid = find_invalid(text, prev);
        }
        if (prev < text.length())
            result.append(text.substr(prev));
        return result;
    }
}

void validate(std::istream& stream)
{
    std::string line;
    int cur = 0;
    while (std::getline(stream, line))
    {
        ++cur;
        auto invalid = find_invalid(line);
        if (invalid != std::string::npos)
        {
            throw exception("Found invalid UTF-8 at line " + std::to_string(cur) + ", column " +
                std::to_string(invalid + 1));
        }
    }
}

}

}
