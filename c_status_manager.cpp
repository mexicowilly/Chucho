/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/status_manager.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/status_manager.h>
#include <vector>
#include <mutex>
#include <algorithm>

namespace
{

class c_status_observer : public chucho::status_observer
{
public:
    static bool equal(const std::shared_ptr<c_status_observer> obs, chucho_status_observer pobs);

    c_status_observer(chucho_status_observer obs);

    chucho_status_observer get_c_observer() const;
    virtual void status_reported(const chucho::status& st) override;

private:
    chucho_status_observer c_observer_;
};

struct static_data
{
    static_data();

    std::vector<std::shared_ptr<c_status_observer>> c_observers_;
};

inline c_status_observer::c_status_observer(chucho_status_observer obs)
    : c_observer_(obs)
{
}

inline bool c_status_observer::equal(const std::shared_ptr<c_status_observer> obs, chucho_status_observer pobs)
{
    return obs->c_observer_ == pobs;
}

inline chucho_status_observer c_status_observer::get_c_observer() const
{
    return c_observer_;
}

void c_status_observer::status_reported(const chucho::status& st)
{
    chucho_status cst;
    chucho::status::level lvl = st.get_level();
    if (lvl == chucho::status::level::INFO_) 
        cst.level = chucho_status_level_info;
    else if (lvl == chucho::status::level::WARNING_) 
        cst.level = chucho_status_level_warning;
    else
        cst.level = chucho_status_level_error;
    cst.message = st.get_message().c_str();
    cst.origin = st.get_origin().c_str();
    cst.time = chucho::status::clock_type::to_time_t(st.get_time());
    c_observer_(&cst);
}

static_data::static_data()
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

static_data& data()
{
    static std::once_flag once;
    // This will be cleaned in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

}

extern "C"
{

chucho_rc chucho_status_add_observer(chucho_status_observer obs)
{
    if (obs == nullptr) 
        return CHUCHO_NULL_POINTER;
    try
    {
        auto cobs = std::make_shared<c_status_observer>(obs);
        data().c_observers_.push_back(cobs);
        chucho::status_manager::get()->add(cobs);
    }
    catch (...) 
    {
        return CHUCHO_OUT_OF_MEMORY;
    }
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_status_clear(void)
{
    chucho::status_manager::get()->clear();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_status_get_count(size_t* cnt)
{
    if (cnt == nullptr) 
        return CHUCHO_NULL_POINTER;
    *cnt = chucho::status_manager::get()->get_count();
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_status_get_level(chucho_status_level* lvl)
{
    if (lvl == nullptr) 
        return CHUCHO_NULL_POINTER;
    auto cpplvl = chucho::status_manager::get()->get_level();
    if (cpplvl == chucho::status::level::INFO_) 
        *lvl = chucho_status_level_info;
    else if (cpplvl == chucho::status::level::WARNING_) 
        *lvl = chucho_status_level_warning;
    else
        *lvl = chucho_status_level_error;
    return CHUCHO_NO_ERROR;
}

chucho_rc chucho_status_remove_observer(chucho_status_observer obs)
{
    if (obs == nullptr) 
        return CHUCHO_NULL_POINTER;
    static_data& sd(data());
    // I was using a lambda here, but the Sun compiler hated it
    auto found = std::find_if(sd.c_observers_.begin(),
                              sd.c_observers_.end(),
                              std::bind(c_status_observer::equal, std::placeholders::_1, obs));
    if (found != sd.c_observers_.end()) 
    {
        chucho::status_manager::get()->remove(*found);
        sd.c_observers_.erase(found);
    }
    return CHUCHO_NO_ERROR;
}

}
