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

#include "logger_creator_item.hpp"
#include "logger_editable_item.hpp"
#include "logger_editor.hpp"

namespace chucho
{

namespace config
{

logger_creator_item::logger_creator_item(QTreeWidget& tree)
    : creator_item(tree, "<Add Logger>")
{
}

QWidget* logger_creator_item::create_editor(QWidget* parent)
{
    return new logger_editor(parent, tree_);
}

void logger_creator_item::create_item(QTreeWidgetItem* parent)
{
    logger_editable_item* item = new logger_editable_item(tree_, "<Type logger name>");
    create_item_impl(parent, item);
}

}

}