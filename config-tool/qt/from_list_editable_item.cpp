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

#include "from_list_editable_item.hpp"
#include <QComboBox>
#include <cstdarg>

namespace chucho
{

namespace config
{

from_list_editable_item::from_list_editable_item(const std::string& key,
                                                 const std::string& value,
                                                 std::size_t count,
                                                 ...)
    : editable_item(key, value),
      manually_editable_(false)
{
    va_list args;
    va_start(args, count);
    for (std::size_t i = 0; i < count; i++)
        lst_.push_back(va_arg(args, const char*));
    va_end(args);
}

QWidget* from_list_editable_item::create_editor(QWidget* parent)
{
    QComboBox* result = new QComboBox(parent);
    QStringList qlst;
    for (const auto& s : lst_)
        qlst << QString::fromStdString(s);
    result->addItems(qlst);
    if (manually_editable_)
        result->setEditable(true);
    return result;
}

}

}
