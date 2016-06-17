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

#if !defined(CHUCHO_CONFIG_LEVEL_EDITABLE_ITEM_HPP__)
#define CHUCHO_CONFIG_LEVEL_EDITABLE_ITEM_HPP__

#include "editable_item.hpp"
#include <chucho/level.hpp>

namespace chucho
{

namespace config
{

class level_editable_item : public editable_item
{
public:
    level_editable_item(const std::string& key, std::shared_ptr<chucho::level> lvl);

    virtual QWidget* create_editor(QWidget* parent) override;
};

}

}


#endif