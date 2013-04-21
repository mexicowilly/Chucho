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

#if !defined(CHUCHO_HOST_HPP__)
#define CHUCHO_HOST_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT host
{
public:
    static const std::string& get_base_name();
    static const std::string& get_full_name();

private:
    CHUCHO_NO_EXPORT static void get_base_impl();
    CHUCHO_NO_EXPORT static void get_full_impl();

    static std::string base_;
    static std::string full_;
};

}

#endif
