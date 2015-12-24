/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_TEXT_UTIL_HPP__)
#define CHUCHO_TEXT_UTIL_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <string>
#include <vector>
#include <cstdarg>

namespace chucho
{

namespace text_util
{

const std::size_t MAX_MESSAGE_SIZE = 500 * 1024;

std::string format(const char* const fmt, va_list args);
std::string to_lower(const std::string& text);
std::vector<std::string> tokenize(const std::string& text, char delim);

}

}

#endif
