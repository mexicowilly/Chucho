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

#if !defined(CHUCHO_SCROLLABLE_HPP__)
#define CHUCHO_SCROLLABLE_HPP__

#include "chucho_curses.h"
#include <cstddef>
#include <vector>
#include <functional>
#include <string>

namespace chucho
{

namespace config_tool
{

class scrollable
{
public:
    scrollable(unsigned x,
               unsigned y,
               std::size_t height,
               std::size_t width,
               const std::vector<std::string>& items,
               std::function<bool(const std::string&)> selected,
               std::function<bool(chtype)> unknown);
    ~scrollable();

    void run();

private:
    void display_arrows() const;
    void highlight_current(bool state);
    void populate();
    void scroll_(int num);

    std::vector<std::string> items_;
    WINDOW* win_;
    std::function<bool(const std::string&)> selected_;
    std::function<bool(chtype)> unknown_;
    std::vector<std::string>::const_iterator current_;
    std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator> displayed_;
};

}

}

#endif
