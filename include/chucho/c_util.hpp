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

#if !defined(CHUCHO_C_UTIL_HPP__)
#define CHUCHO_C_UTIL_HPP__

#include <chucho/c_level.hpp>

namespace chucho
{

namespace c_util
{

const chucho_level* cpp_level_to_c(std::shared_ptr<chucho::level> lvl);

}

}

#endif