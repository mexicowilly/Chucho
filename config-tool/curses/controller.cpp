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

#include "controller.hpp"

namespace chucho
{

namespace config_tool
{

controller::controller(const properties& props)
    : props_(props)
{
    initscr();
    noecho();
    cbreak();
    title_ = newwin(1, COLS, 0, 0);
    wprintw(title_, "Chucho Loggers");
    wrefresh(title_);
    loggers_ = new loggers_win(0, 1, COLS, 5);
}

controller::~controller()
{
    delwin(title_);
    delete loggers_;
    endwin();
}

void controller::run()
{
    loggers_->run();
}

}

}
