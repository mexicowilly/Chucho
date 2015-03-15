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

#if !defined(CHUCHO_POP_UP_WIN_HPP__)
#define CHUCHO_POP_UP_WIN_HPP__

#include "chucho_curses.h"
#include <string>

namespace chucho
{

namespace config_tool
{

class pop_up_win
{
public:
    pop_up_win(unsigned x, unsigned y, std::size_t width, const std::string& text);
    ~pop_up_win();

    void run();

private:
    WINDOW* win_;
};

}

}

#endif
