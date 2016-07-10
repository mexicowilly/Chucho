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
#include "emittable.hpp"
#include <chucho/loggable.hpp>
#include <chucho/log.hpp>
#include <QComboBox>
#include <QKeyEvent>
#include <QKeySequence>
#include <iomanip>

namespace chucho
{

namespace config
{

creator_from_list_item::creator_from_list_item(QTreeWidget& tree, const std::string& text, bool disable_on_create)
    : creator_item(tree, text, disable_on_create),
      created_(nullptr)
{
    rename_logger(typeid(*this));
}

void creator_from_list_item::commit_data(QWidget* ed)
{
    if (created_ != nullptr)
    {
        auto it = creators_.find(created_->text(0));
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
        created_ = nullptr;
    }
    disconnect(tree_.itemDelegate(), SIGNAL(commitData(QWidget*)), this, SLOT(commit_data(QWidget*)));
}

QWidget* creator_from_list_item::create_editor(QWidget* parent)
{
    QComboBox* box = new QComboBox(parent);
    QStringList keys;
    for (const auto& p : creators_)
        keys << p.first;
    box->addItems(keys);
    connect(tree_.itemDelegate(), SIGNAL(commitData(QWidget*)), SLOT(commit_data(QWidget*)));
    return box;
}

void creator_from_list_item::create_item(QTreeWidgetItem* parent)
{
    created_ = new QTreeWidgetItem();
    created_->setFlags(created_->flags() | Qt::ItemIsEditable);
    create_item_impl(parent, created_);
}

}

}