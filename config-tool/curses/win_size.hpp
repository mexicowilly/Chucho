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


#if !defined(CHUCHO_WIN_SIZE_HPP__)
#define CHUCHO_WIN_SIZE_HPP__

#include "chucho_curses.h"
#include <cstddef>

namespace chucho
{

namespace config_tool
{

class win_size
{
public:
    win_size(WINDOW* w);

    std::size_t get_height() const;
    std::size_t get_width() const;
    unsigned get_x() const;
    unsigned get_y() const;

private:
    unsigned x_;
    unsigned y_;
    std::size_t height_;
    std::size_t width_;
};

inline win_size::win_size(WINDOW* w)
{
    getmaxyx(w, height_, width_);
    getyx(w, y_, x_);
}

inline std::size_t win_size::get_height() const
{
    return height_;
}

inline std::size_t win_size::get_width() const
{
    return width_;
}

inline unsigned win_size::get_x() const
{
    return x_;
}

inline unsigned win_size::get_y() const
{
    return y_;
}

}

}

#endif
