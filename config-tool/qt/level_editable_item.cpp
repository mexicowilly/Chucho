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

#include "level_editable_item.hpp"
#include <QComboBox>

namespace chucho
{

namespace config
{

level_editable_item::level_editable_item(const std::string& key, std::shared_ptr<chucho::level> lvl)
    : editable_item(key, lvl ? lvl->get_name() : "")
{
}

QWidget* level_editable_item::create_editor(QWidget* parent)
{
    QComboBox* result = new QComboBox(parent);
    result->addItems(QStringList() << "" << "TRACE" << "DEBUG" << "INFO" << "WARN" << "ERROR" << "FATAL" << "OFF");
    result->setEditable(true);
    return result;
}

}

}