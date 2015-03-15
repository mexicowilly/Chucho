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

#include "pop_up_win.hpp"
#include "util.hpp"

namespace chucho
{

namespace config_tool
{

pop_up_win::pop_up_win(unsigned x, unsigned y, std::size_t width, const std::string& text)
{
    auto lines = util::line_break(text, width - 2);
    win_ = newwin(lines.size() + 2, width, y, x);
    box(win_, 0, 0);
    for (unsigned i = 0; i < lines.size(); i++)
        mvwaddstr(win_, i + 1, 1, lines[i].c_str());
    wrefresh(win_);
    keypad(win_, TRUE);
}

pop_up_win::~pop_up_win()
{
    delwin(win_);
}

void pop_up_win::run()
{
    wgetch(win_);
}

}

}
