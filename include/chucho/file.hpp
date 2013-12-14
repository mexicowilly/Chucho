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

#if !defined(CHUCHO_FILE_HPP__)
#define CHUCHO_FILE_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <string>
#include <cstdint>

namespace chucho
{

namespace file
{

CHUCHO_PRIV_EXPORT extern const char dir_sep;

CHUCHO_PRIV_EXPORT std::string base_name(const std::string& name);
CHUCHO_PRIV_EXPORT void create_directories(const std::string& name);
CHUCHO_PRIV_EXPORT void create_directory(const std::string& name);
CHUCHO_PRIV_EXPORT std::string directory_name(const std::string& name);
CHUCHO_PRIV_EXPORT bool exists(const std::string& name);
CHUCHO_PRIV_EXPORT bool is_fully_qualified(const std::string& name);
CHUCHO_PRIV_EXPORT void remove(const std::string& name);
CHUCHO_PRIV_EXPORT void remove_all(const std::string& name);
CHUCHO_PRIV_EXPORT std::uintmax_t size(const std::string& name);

}

}

#endif
