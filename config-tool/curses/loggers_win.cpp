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

#include "loggers_win.hpp"

namespace
{
}

namespace chucho
{

namespace config_tool
{

loggers_win::loggers_win(unsigned x,
                         unsigned y,
                         std::size_t width,
                         std::size_t height)
    : scrollable(x, y, width, height, std::vector<std::string>(1, "+"))
{
    rename_logger(typeid(*this));
}

loggers_win::exit_status loggers_win::selected()
{
    static unsigned num = 1;

    if (get_current() == "+")
        push_before_back(std::to_string(num++));
    return exit_status::should_not_exit; 
}

loggers_win::exit_status loggers_win::unknown(chtype ch)
{
    exit_status st = exit_status::should_not_exit;
    switch (ch)
    {
    case 'q':
        st = exit_status::should_exit;
        break;

    case 'r':
        if (get_current() != "+")
            remove(get_current());
        break; 
    }
    return st;
}

}

}
