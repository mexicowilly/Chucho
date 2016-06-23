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

#include "writer_creator_item.hpp"
#include "cout_writer_item.hpp"
#include <QComboBox>

namespace chucho
{

namespace config
{

writer_creator_item::writer_creator_item(QTreeWidget& tree)
    : creator_item(tree, "<Add Writer>"),
      created_(nullptr)
{
    creators_["ActiveMQ Writer"] = nullptr;
    creators_["Asynchronous Writer"] = nullptr;
    creators_["DB2 Writer"] = nullptr;
    creators_["Door Writer"] = nullptr;
    creators_["Email Writer"] = nullptr;
    creators_["File Writer"] = nullptr;
    creators_["MySQL Writer"] = nullptr;
    creators_["Named Pipe Writer"] = nullptr;
    creators_["Oracle Writer"] = nullptr;
    creators_["Pipe Writer"] = nullptr;
    creators_["PostgresSQL Writer"] = nullptr;
    creators_["Remote Chucho Writer"] = nullptr;
    creators_["Rolling File Writer"] = nullptr;
    creators_["SQLite Writer"] = nullptr;
    creators_["Standard Error Writer"] = nullptr;
    creators_["Standard Output Writer"] = &writer_creator_item::create_writer_item<cout_writer_item>;
    creators_["Syslog Writer"] = nullptr;
    creators_["Windows Event Log Writer"] = nullptr;
    creators_["ZeroMQ Writer"] = nullptr;
}

QWidget* writer_creator_item::create_editor(QWidget* parent)
{
    QComboBox* box = new QComboBox(parent);
    QStringList keys;
    for (const auto& p : creators_)
        keys << p.first;
    box->addItems(keys);
    connect(box, SIGNAL(activated(const QString&)), SLOT(item_activated(const QString&)));
    return box;
}

void writer_creator_item::create_item(QTreeWidgetItem* parent)
{
    created_ = new QTreeWidgetItem(QStringList() << "<Choose writer>" << "");
    created_->setFlags(created_->flags() | Qt::ItemIsEditable);
    create_item_impl(parent, created_);
}

void writer_creator_item::item_activated(const QString& text)
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