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

#include "level_threshold_filter_item.hpp"
#include "level_editable_item.hpp"

namespace chucho
{

namespace config
{

level_threshold_filter_item::level_threshold_filter_item(QTreeWidget&)
{
    level_ = new level_editable_item("Level", chucho::level::INFO_());
    addChild(level_);
}

void level_threshold_filter_item::emit_config(std::ostream& stream, std::size_t tabstop)
{
    indent(stream, tabstop) << "- chucho::level_threshold_filter:" << std::endl;
    indent(stream, tabstop + 1) << " - level: " << level_->text(1) << std::endl;
}

}

}