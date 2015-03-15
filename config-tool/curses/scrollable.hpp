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
#include "win_size.hpp"
#include "emitter.hpp"
#include <chucho/loggable.hpp>
#include <cstddef>
#include <vector>
#include <functional>
#include <string>

namespace chucho
{

namespace config_tool
{

class scrollable : protected loggable<scrollable>
{
public:
    struct item
    {
        item(const std::string& text, std::shared_ptr<emitter> emit = std::shared_ptr<emitter>());
        item(const char* const text, std::shared_ptr<emitter> emit = std::shared_ptr<emitter>());

        std::string text_;
        std::shared_ptr<emitter> emitter_;
    };

    scrollable(const std::string& title,
               unsigned x,
               unsigned y,
               std::size_t width,
               std::size_t height);
    scrollable(unsigned x,
               unsigned y,
               std::size_t width,
               std::size_t height);
    virtual ~scrollable();

    void push_before_back(const item& itm);
    void remove_current();
    void replace_current(const item& itm);
    void run();
    void set_items(const std::vector<item>& items);

protected:
    enum class exit_status
    {
        should_exit,
        should_not_exit
    };

    const item& get_current() const;
    win_size get_win_size();
    virtual void highlighted();
    virtual exit_status selected();
    virtual exit_status unknown(chtype ch);

private:
    enum class refresh_status
    {
        should_refresh,
        should_not_refresh
    };

    void display_arrows();
    void highlight_current(bool state) const;
    void populate();
    void scroll_(int num);
    void set_displayed_second();
    void update(refresh_status refresh);

    std::vector<item> items_;
    WINDOW* win_;
    WINDOW* title_;
    std::vector<item>::iterator current_;
    std::pair<std::vector<item>::iterator, std::vector<item>::iterator> displayed_;
};

inline const scrollable::item& scrollable::get_current() const
{
    return *current_;
}

inline win_size scrollable::get_win_size()
{
    return win_size(win_);
}

}

}

#endif
