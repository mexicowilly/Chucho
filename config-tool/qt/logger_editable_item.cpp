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

#include "logger_editable_item.hpp"
#include "boolean_editable_item.hpp"
#include "level_editable_item.hpp"
#include "logger_editor.hpp"

namespace chucho
{

namespace config
{

logger_editable_item::logger_editable_item(QTreeWidget& tree, const std::string& name)
    : editable_item(name, ""),
      tree_(tree)
{
    rename_logger(typeid(*this));
    QTreeWidgetItem* child = new level_editable_item("Level", std::shared_ptr<chucho::level>());
    addChild(child);
    child = new boolean_editable_item("Writes to Ancestors", true);
    addChild(child);
    child = new QTreeWidgetItem(QStringList() << "<Add Writer>" << "");
    addChild(child);
}

int logger_editable_item::column() const
{
    return 0;
}

QWidget* logger_editable_item::create_editor(QWidget* parent)
{
    return new logger_editor(parent, tree_);
}

void logger_editable_item::emit_config(std::ostream& stream)
{
}

}

}