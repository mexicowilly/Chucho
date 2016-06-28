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

#include "creator_from_list_item.hpp"
#include <QComboBox>

namespace chucho
{

namespace config
{

creator_from_list_item::creator_from_list_item(QTreeWidget& tree, const std::string& text, bool disable_on_create)
    : creator_item(tree, text, disable_on_create),
      created_(nullptr)
{
}

QWidget* creator_from_list_item::create_editor(QWidget* parent)
{
    QComboBox* box = new QComboBox(parent);
    QStringList keys;
    for (const auto& p : creators_)
        keys << p.first;
    box->addItems(keys);
    connect(box, SIGNAL(activated(const QString&)), SLOT(item_activated(const QString&)));
    return box;
}

void creator_from_list_item::create_item(QTreeWidgetItem* parent)
{
    created_ = new QTreeWidgetItem();
    created_->setFlags(created_->flags() | Qt::ItemIsEditable);
    create_item_impl(parent, created_);
}

void creator_from_list_item::item_activated(const QString& text)
{
    if (created_ != nullptr)
    {
        auto it = creators_.find(text);
        if (it != creators_.end() && it->second != nullptr)
        {
            auto p = created_->parent();
            int idx = p->indexOfChild(created_);
            delete p->takeChild(idx);
            auto child = std::bind(it->second, this)();
            child->setText(0, it->first);
            p->insertChild(idx, child);
            child->setExpanded(true);
            tree_.setCurrentItem(child);
        }
    }
}

}

}