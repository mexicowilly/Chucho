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

#include "logger_editable_item.hpp"
#include "boolean_editable_item.hpp"
#include "level_editable_item.hpp"
#include "logger_editor.hpp"
#include "writer_creator_item.hpp"
#include "writer_item.hpp"

namespace chucho
{

namespace config
{

logger_editable_item::logger_editable_item(QTreeWidget& tree, const std::string& name)
    : editable_item(name, ""),
      tree_(tree)
{
    rename_logger(typeid(*this));
    level_ = new level_editable_item("Level", std::shared_ptr<chucho::level>());
    addChild(level_);
    writes_to_ancestors_ = new boolean_editable_item("Writes to Ancestors", true);
    addChild(writes_to_ancestors_);
    auto child = new writer_creator_item(tree);
    addChild(child);
}

int logger_editable_item::column() const
{
    return 0;
}

QWidget* logger_editable_item::create_editor(QWidget* parent)
{
    return new logger_editor(parent, tree_);
}

void logger_editable_item::emit_config(std::ostream& stream, std::size_t tabstop)
{
    indent(stream, tabstop++) << "- chucho::logger:" << std::endl;
    indent(stream, tabstop) << "- name: '" << text(0) << '\'' << std::endl;
    if (!level_->text(1).isEmpty())
        indent(stream, tabstop) << "- level: '" << level_->text(1) << '\'' << std::endl;
    indent(stream, tabstop) << "- writes_to_ancestors: " << writes_to_ancestors_->text(1) << std::endl;
    for (int i = 0; i < childCount(); i++)
    {
        auto wi = dynamic_cast<writer_item*>(child(i));
        if (wi)
            wi->emit_config(stream, tabstop);
    }
}

}

}