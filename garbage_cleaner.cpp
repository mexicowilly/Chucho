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

#include <chucho/garbage_cleaner.hpp>
#include <mutex>

namespace
{

std::once_flag once;
// This gets cleaned by finalize();
chucho::garbage_cleaner* gc;

void init()
{
    gc = new chucho::garbage_cleaner();
}

}

namespace chucho
{

garbage_cleaner* gc(new garbage_cleaner());

garbage_cleaner::~garbage_cleaner()
{
    for (auto cln : cleaners_)
        cln();
}

void garbage_cleaner::add(cleaner_type cln)
{
    cleaners_.push_back(cln);
}

garbage_cleaner& garbage_cleaner::get()
{
    std::call_once(once, init);
    return *gc;
}

}
