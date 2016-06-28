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

#include "number_editable_item.hpp"
#include <QLineEdit>
#include <QIntValidator>

namespace chucho
{

namespace config
{

number_editable_item::number_editable_item(const std::string& key, int value, int minimum, int maximum)
    : editable_item(key, std::to_string(value)),
      minimum_(minimum),
      maximum_(maximum)
{
}

QWidget* number_editable_item::create_editor(QWidget* parent)
{
    auto le = new QLineEdit(parent);
    le->setValidator(new QIntValidator(minimum_, maximum_));
    return le;
}

}

}