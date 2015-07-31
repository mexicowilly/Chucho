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

#include <chucho/garbage_cleaner.hpp>

namespace chucho
{

garbage_cleaner::~garbage_cleaner()
{
    for (auto cln : cleaners_)
        cln();
}

void garbage_cleaner::add(cleaner_type cln)
{
    std::lock_guard<std::mutex> lg(guard_);
    cleaners_.push_back(cln);
}

garbage_cleaner& garbage_cleaner::get()
{
    static std::once_flag once;
    // This gets cleaned by finalize
    static garbage_cleaner* gc;

    std::call_once(once, [&] () { gc = new garbage_cleaner(); });
    return *gc;
}

}
