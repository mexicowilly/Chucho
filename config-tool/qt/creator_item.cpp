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

#include "creator_item.hpp"
#include <chucho/log.hpp>

namespace chucho
{

namespace config
{

creator_item::creator_item(QTreeWidget& tree, const std::string& text)
    : editable_item(text, ""),
      tree_(tree)
{
    setFlags(flags() & ~Qt::ItemIsEditable);
}

int creator_item::column() const
{
    return 0;
}

void creator_item::create_item_impl(QTreeWidgetItem* parent, QTreeWidgetItem* item)
{
    if (parent == nullptr)
        tree_.insertTopLevelItem(tree_.topLevelItemCount() - 1, item);
    else
        parent->insertChild(parent->childCount() - 1, item);
    item->setExpanded(true);
    tree_.editItem(item);
    tree_.setCurrentItem(item);
}

}

}