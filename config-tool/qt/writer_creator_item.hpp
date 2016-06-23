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

#if !defined(CHUCHO_CONFIG_WRITER_CREATOR_ITEM_HPP__)
#define CHUCHO_CONFIG_WRITER_CREATOR_ITEM_HPP__

#include "creator_item.hpp"
#include <map>

namespace chucho
{

namespace config
{

class writer_creator_item : public QObject, public creator_item
{
    Q_OBJECT

public:
    typedef QTreeWidgetItem* (writer_creator_item::* creator_func)();

    writer_creator_item(QTreeWidget& tree);

    virtual QWidget* create_editor(QWidget* parent) override;
    virtual void create_item(QTreeWidgetItem* parent) override;

public slots:
    void item_activated(const QString& text);

private:
    template <typename type>
    QTreeWidgetItem* create_writer_item();

    std::map<QString, creator_func> creators_;
    QTreeWidgetItem* created_;
};

template <typename type>
QTreeWidgetItem* writer_creator_item::create_writer_item()
{
    return new type();
}

}

}

#endif