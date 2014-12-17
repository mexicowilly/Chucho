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

#include "scrollable.hpp"
#include <chucho/exception.hpp>

namespace chucho
{

namespace config_tool
{

scrollable::scrollable(unsigned x,
                       unsigned y,
                       std::size_t height,
                       std::size_t width,
                       const std::vector<std::string>& items,
                       std::function<bool(const std::string&)> selected,
                       std::function<bool(chtype)> unknown)
    : items_(items),
      win_(newwin(height, width, y, x)),
      selected_(selected),
      unknown_(unknown)
{
    if (height < 3 || width < 5)
        throw exception("Height must be three or greater and width must be five or greater");
    populate();
}

scrollable::~scrollable()
{
    delwin(win_);
}

void scrollable::display_arrows() const
{
    std::size_t width;
    std::size_t height;
    getmaxyx(win_, height, width);
    mvwvline(win_, 1, 1, ' ', height - 2);
    if (displayed_.first != items_.begin())
        mvwaddch(win_, 1, 1, ACS_UARROW);
    if (displayed_.second != items_.end() - 1)
        mvwaddch(win_, height - 1, 1, ACS_DARROW);
}

void scrollable::highlight_current(bool state)
{
    auto idx = current_ - displayed_.first;
    std::size_t width;
    std::size_t height;
    getmaxyx(win_, height, width);
    mvwchgat(win_, idx + 1, 1, width - 2, (state ? A_STANDOUT : A_NORMAL), 0, nullptr);
}

void scrollable::populate()
{
    std::size_t width;
    std::size_t height;
    getmaxyx(win_, height, width);
    box(win_, 0, 0);
    std::size_t limit = std::min(height, items_.size()) - 2;
    for (std::size_t i = 1; i < limit; i++)
        mvwprintw(win_, i, 0, "  %s", items_[i - 1].c_str());
    displayed_.first = items_.begin();
    displayed_.second = displayed_.first + limit;
    current_ = displayed_.first;
    display_arrows();
    highlight_current(true);
    wrefresh(win_);
}

void scrollable::run()
{
    while (true)
    {
        int ch = wgetch(win_);
        switch (ch)
        {
        case KEY_DOWN:
            if (current_ == displayed_.second &&
                current_ + 1 != items_.end())
            {
                scroll_(-1);
            }
            else
            {
                highlight_current(false);
                current_++;
                highlight_current(true);
            }
            wrefresh(win_);
            break; 

        case KEY_UP:
            if (current_ == displayed_.first &&
                current_ != items_.end())
            {
                scroll_(1);
            }
            else
            {
                highlight_current(false);
                current_--;
                highlight_current(true);
            }
            wrefresh(win_);
            break; 

        case KEY_ENTER:
            if (selected_(*current_))
                return;
            break; 

        default:
            if (unknown_(ch))
                return;
            break;
        }
        ch = wgetch(win_);
    }
}

void scrollable::scroll_(int num)
{
    highlight_current(false);
    wscrl(win_, num);
    current_ += num;
    displayed_.first += num;
    displayed_.second += num;
    display_arrows();
    highlight_current(true);
}

}

}
