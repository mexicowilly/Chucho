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
#include "chucho_keys.hpp"
#include <chucho/exception.hpp>
#include <chucho/log.hpp>
#include <cstdlib>

namespace chucho
{

namespace config_tool
{

scrollable::scrollable(const std::string& title,
                       unsigned x,
                       unsigned y,
                       std::size_t width,
                       std::size_t height)
    : win_(nullptr),
      title_(nullptr)
{
    if (height < 4 || width < 5)
        throw exception("Height must be four or greater and width must be five or greater");
    if (!title.empty())
    {
        title_ = newwin(1, width, y, x);
        waddstr(title_, title.c_str());
        wrefresh(title_);
        y += 1;
        height -= 1;
    }
    win_ = newwin(height, width, y, x);
    keypad(win_, TRUE);
}

scrollable::scrollable(unsigned x,
                       unsigned y,
                       std::size_t width,
                       std::size_t height)
    : scrollable("", x, y, width, height)
{
}

scrollable::~scrollable()
{
    if (title_ != nullptr)
        delwin(title_);
    delwin(win_);
}

void scrollable::display_arrows()
{
    win_size dim(win_);
    mvwvline(win_, 1, 1, ' ', dim.get_height() - 2);
    // ACS_UARROW and ACS_DARROW don't come out right on my Macintosh,
    // so I just hard code ^ and v.
    if (displayed_.first != items_.begin())
        mvwaddch(win_, 1, 1, '^');
    if (std::distance(displayed_.second, items_.end()) > 1)
        mvwaddch(win_, dim.get_height() - 2, 1, 'v');
    // For some reason drawing the line can erase a piece of the box,
    // even though the line never touches the box character
    mvwhline(win_, 0, 1, ACS_HLINE, 2);
    mvwhline(win_, dim.get_height() - 1, 1, ACS_HLINE, 2);
}

void scrollable::highlight_current(bool state) const
{
    auto idx = std::distance(displayed_.first, current_);
    win_size dim(win_);
    mvwchgat(win_, idx + 1, 1, dim.get_width() - 2, (state ? A_STANDOUT : A_NORMAL), 0, nullptr);
}

void scrollable::highlighted()
{
}

void scrollable::populate()
{
    win_size dim(win_);
    std::size_t limit = std::min(dim.get_height() - 2, items_.size()) - 1;
    current_ = items_.begin();
    displayed_.first = current_;
    displayed_.second = displayed_.first;
    std::advance(displayed_.second, std::min(dim.get_height() - 2, items_.size()) - 1);
    update(refresh_status::should_refresh);
}

void scrollable::push_before_back(const item& itm)
{
    if (items_.empty())
    {
        CHUCHO_ERROR_LGBL("You cannot push_before_back to an empty scrollable");
    }
    else
    {
        auto disp_idx = std::distance(items_.begin(), displayed_.first);
        current_ = items_.insert(items_.end() - 1, itm);
        displayed_.first = items_.begin();
        std::advance(displayed_.first, disp_idx);
        set_displayed_second();
        update(refresh_status::should_refresh);
    }
}

void scrollable::remove_current()
{
    auto disp_idx = std::distance(items_.begin(), displayed_.first);
    current_ = items_.erase(current_);
    if (!items_.empty() && current_ != items_.begin())
        current_--;
    displayed_.first = items_.begin();
    if (current_ != items_.begin())
        std::advance(displayed_.first, disp_idx); 
    if (displayed_.first > current_)
        displayed_.first = current_;
    set_displayed_second(); 
    update(refresh_status::should_refresh);
}

void scrollable::replace_current(const item& itm)
{
    *current_ = itm;
    update(refresh_status::should_refresh);
}

void scrollable::run()
{
    while (true)
    {
        int ch = wgetch(win_);
        CHUCHO_INFO_LGBL("Got key " << ch);
        switch (ch)
        {
        case KEY_DOWN:
            if (current_ + 1 != items_.end())
            {
                if (current_ == displayed_.second)
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
                highlighted();
            }
            break; 

        case KEY_UP:
            if (current_ != items_.begin())
            {
                if (current_ == displayed_.first)
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
                highlighted();
            }
            break; 

        case KEY_ENTER:
        case static_cast<int>(key::ENTER):
            if (selected() == exit_status::should_exit)
                return;
            break; 

        default:
            if (unknown(ch) == exit_status::should_exit)
                return;
            break;
        }
    }
}

void scrollable::scroll_(int num)
{
    if (num == 0)
        return;
    // This method only gets called after bounds checking has been
    // performed, so there is no need to check validity here
    current_ -= num;
    displayed_.first -= num;
    set_displayed_second();
    update(refresh_status::should_not_refresh);
}

void scrollable::set_displayed_second()
{
    displayed_.second = displayed_.first;
    win_size dim(win_);
    std::advance(displayed_.second,
                 std::min(std::distance(displayed_.first, items_.end()),
                          std::distance(displayed_.first, displayed_.first + (dim.get_height() - 2))) - 1);
}

void scrollable::set_items(const std::vector<item>& items)
{
    items_ = items;
    populate();
}

scrollable::exit_status scrollable::selected()
{
    return exit_status::should_not_exit;
}

scrollable::exit_status scrollable::unknown(chtype ch)
{
    return exit_status::should_not_exit;
}

void scrollable::update(refresh_status refresh)
{
    wclear(win_);
    for (auto i = displayed_.first; i <= displayed_.second; i++)
        mvwprintw(win_, std::distance(displayed_.first, i) + 1, 1, "  %s", (*i).text_.c_str());
    // Don't draw the box first. Things can get weird.
    box(win_, 0, 0);
    display_arrows(); 
    highlight_current(true);
    if (refresh == refresh_status::should_refresh)
        wrefresh(win_);
}

scrollable::item::item(const std::string& text, std::shared_ptr<emitter> emit)
    : text_(text),
      emitter_(emit)
{
}

scrollable::item::item(const char* const text, std::shared_ptr<emitter> emit)
    : text_(text),
      emitter_(emit)
{
}

}

}
