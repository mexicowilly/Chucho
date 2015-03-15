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

#include "logger_win.hpp"
#include "pop_up_win.hpp"
#include <map>

namespace
{

class level_win : public chucho::config_tool::cancellable
{
public:
    level_win(unsigned x, unsigned y);

    std::shared_ptr<chucho::level> get_level() const;
    std::string get_level_text() const;

protected:
    virtual exit_status selected() override;

private:
    std::map<std::string, std::shared_ptr<chucho::level>> levels_;
    std::map<std::string, std::shared_ptr<chucho::level>>::iterator sel_;
};

level_win::level_win(unsigned x, unsigned y)
    : cancellable("Level", x, y, 9, 11),
      levels_({
                { "<none>", std::shared_ptr<chucho::level>() },
                { chucho::level::TRACE_()->get_name(), chucho::level::TRACE_() },
                { chucho::level::DEBUG_()->get_name(), chucho::level::DEBUG_() },
                { chucho::level::INFO_()->get_name(), chucho::level::INFO_() },
                { chucho::level::WARN_()->get_name(), chucho::level::WARN_() },
                { chucho::level::ERROR_()->get_name(), chucho::level::ERROR_() },
                { chucho::level::FATAL_()->get_name(), chucho::level::FATAL_() },
                { chucho::level::OFF_()->get_name(), chucho::level::OFF_() }
              }),
      sel_(levels_.end())
{
    std::vector<item> items;
    for (auto p : levels_)
        items.push_back(p.first);
    set_items(items); 
}

std::shared_ptr<chucho::level> level_win::get_level() const
{
    return sel_ == levels_.end() ? std::shared_ptr<chucho::level>() : sel_->second;
}

std::string level_win::get_level_text() const
{
    return sel_ == levels_.end() ? std::string() : sel_->first;
}

level_win::exit_status level_win::selected()
{
    sel_ = levels_.find(get_current().text_);
    return exit_status::should_exit;
}

}

namespace chucho
{

namespace config_tool
{

logger_win::logger_win(const logger_emitter& emitter,
                       unsigned x,
                       unsigned y,
                       std::size_t width,
                       std::size_t height)
    : cancellable(x, y, width, height),
      emitter_(emitter)
{
    rename_logger(typeid(*this));
    std::vector<item> items;
    items.push_back("Name: ");
    if (emitter.get_name())
    {
        if (emitter.get_name()->empty())
            items.back().text_ += "<root>";
        else
            items.back().text_ += *emitter.get_name();
    }
    items.push_back("Level: ");
    if (emitter.get_level())
        items.back().text_ += emitter.get_level()->get_name();
    else
        items.back().text_ += "<none>";
    items.push_back(std::string("Writes to Ancestors: ") +
        (emitter.get_writes_to_ancestors() ? "true" : "false"));
    for (auto we : emitter.get_writer_emitters())
        items.push_back(item(we->get_name(), we));
    items.push_back("+ Writer");
    set_items(items); 
}

logger_win::exit_status logger_win::selected()
{
    if (get_current().text_.find("Name") == 0)
    {
    }
    else if (get_current().text_.find("Level") == 0)
    {
        win_size dim = get_win_size();
        std::shared_ptr<level> lvl;
        std::string txt;
        bool cancelled;
        {
            level_win lw(dim.get_x() + 3, dim.get_y() + 3);
            lw.run();
            lvl = lw.get_level();
            txt = lw.get_level_text();
            cancelled = lw.cancelled();
        }
        if (!cancelled)
        {
            emitter_->set_level(lvl); 
            replace_current("Level: " + txt);
        }
    }
    else if (get_current().text_.find("Writes to Ancestors") == 0)
    {
    }
    else if (get_current().text_ == "+ Writer")
    {
    }
    else
    {
        // This is a writer emitter
    }
    return exit_status::should_not_exit; 
}

logger_win::exit_status logger_win::unknown(chtype ch)
{
    exit_status st = cancellable::unknown(ch);
    if (cancelled())
    {
        emitter_ = optional<logger_emitter>();
    }
    else if (ch == 's')
    {
        auto p = emitter_->is_valid();
        if (p.first)
        {
            st = exit_status::should_exit;
        }
        else
        {
            std::size_t w = COLS / 2;
            pop_up_win pop(w - (w / 2), (LINES / 2) - 5, w, p.second);
            pop.run();
        }
    }
    return st; 
}

}

}
