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

#include <chucho/demangle.hpp>
#if defined(__clang__) || defined(__GNUC__)
#include <cxxabi.h>
#include <cstdlib>
#endif

namespace chucho
{

namespace demangle
{

std::string get_demangled_name(const std::type_info& info)
{
#if defined(__clang__) || defined(__GNUC__)
    // On FreeBSD, the __cxa_demangle function sometimes will simply
    // not touch status on success, so we make sure to set it to
    // zero first.
    int status = 0;
    char* demangled = ::abi::__cxa_demangle(info.name(), nullptr, nullptr, &status);
    if (status != 0)
        return info.name();
    std::string result(demangled);
    std::free(demangled);
    return result;
#else
    return info.name();
#endif
}

}

}
