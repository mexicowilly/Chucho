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

#if !defined(CHUCHO_CALENDAR_HPP__)
#define CHUCHO_CALENDAR_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <ctime>
#include <string>

namespace chucho
{

namespace calendar
{

CHUCHO_EXPORT std::string format(const struct std::tm& cal,
                                 const std::string& pattern);
CHUCHO_EXPORT struct std::tm get_local(std::time_t t);
CHUCHO_EXPORT struct std::tm get_utc(std::time_t t);

}

}

#endif
