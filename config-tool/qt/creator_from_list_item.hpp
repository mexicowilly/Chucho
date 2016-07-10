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

#if !defined(CHUCHO_CONFIG_CREATOR_FROM_LIST_ITEM_HPP__)
#define CHUCHO_CONFIG_CREATOR_FROM_LIST_ITEM_HPP__

#include "creator_item.hpp"
#include <map>

namespace chucho
{

namespace config
{

class creator_from_list_item : public QObject, public creator_item
{
    Q_OBJECT

public:
    creator_from_list_item(QTreeWidget& tree, const std::string& text, bool disable_on_create = false);

    virtual QWidget* create_editor(QWidget* parent) override;
    virtual void create_item(QTreeWidgetItem* parent) override;

protected:
    typedef QTreeWidgetItem* (creator_from_list_item::* creator_func)();

    template <typename type>
    QTreeWidgetItem* create_replacement_item();
    template <typename type>
    creator_func get_func();

    std::map<QString, creator_func> creators_;

private slots:
    void commit_data(QWidget* ed);

private:
    QTreeWidgetItem* created_;
};

template <typename type>
QTreeWidgetItem* creator_from_list_item::create_replacement_item()
{
    return new type(tree_);
}

template <typename type>
inline creator_from_list_item::creator_func creator_from_list_item::get_func()
{
    return &creator_from_list_item::create_replacement_item<type>;
}

}

}

#endif
