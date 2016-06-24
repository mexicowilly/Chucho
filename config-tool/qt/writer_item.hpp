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

#if !defined(CHUCHO_CONFIG_WRITER_ITEM_HPP__)
#define CHUCHO_CONFIG_WRITER_ITEM_HPP__

#include "emittable.hpp"
#include <QTreeWidgetItem>

namespace chucho
{

namespace config
{

class writer_item : public QTreeWidgetItem, public emittable
{
public:
    writer_item(QTreeWidget& tree);

    virtual void emit_config(std::ostream& stream);
};

}

}

#endif