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

#include "text_editable_item.hpp"
#include <QLineEdit>

namespace chucho
{

namespace config
{

text_editable_item::text_editable_item(const std::string& key, const std::string& value)
    : editable_item(key, value)
{
}

QWidget* text_editable_item::create_editor(QWidget* parent)
{
    return new QLineEdit(parent);
}

}

}