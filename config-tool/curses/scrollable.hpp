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
    scrollable(const std::string& title,
               unsigned x,
               unsigned y,
               std::size_t width,
               std::size_t height,
               const std::vector<std::string>& items);
    scrollable(unsigned x,
               unsigned y,
               std::size_t width,
               std::size_t height,
               const std::vector<std::string>& items);
    ~scrollable();

    void push_before_back(const std::string& item);
    void remove(const std::string& item);
    void run();

protected:
    enum class exit_status
    {
        should_exit,
        should_not_exit
    };

    const std::string& get_current() const;
    virtual exit_status selected();
    virtual exit_status unknown(chtype ch);

private:
    enum class refresh_status
    {
        should_refresh,
        should_not_refresh
    };

    void display_arrows() const;
    void highlight_current(bool state) const;
    void populate();
    void scroll_(int num);
    void set_displayed_second();
    void update(refresh_status refresh) const;

    std::vector<std::string> items_;
    WINDOW* win_;
    WINDOW* title_;
    std::vector<std::string>::const_iterator current_;
    std::pair<std::vector<std::string>::const_iterator, std::vector<std::string>::const_iterator> displayed_;
};

inline const std::string& scrollable::get_current() const
{
    return *current_;
}

}

}

#endif
