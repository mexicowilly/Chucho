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

#if !defined(CHUCHO_GARBAGE_CLEANER_HPP__)
#define CHUCHO_GARBAGE_CLEANER_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <functional>
#include <vector>
#include <mutex>

namespace chucho
{

class garbage_cleaner
{
public:
    typedef std::function<void()> cleaner_type;

    static garbage_cleaner& get();

    ~garbage_cleaner();

    void add(cleaner_type cln);

private:
    std::vector<cleaner_type> cleaners_;
    std::mutex guard_;
};

}

#endif