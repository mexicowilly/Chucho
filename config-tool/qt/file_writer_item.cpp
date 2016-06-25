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

#include "file_writer_item.hpp"
#include "boolean_editable_item.hpp"
#include "from_list_editable_item.hpp"
#include "text_editable_item.hpp"

namespace
{

class on_start_editable_item : public chucho::config::from_list_editable_item
{
public:
    on_start_editable_item()
        : from_list_editable_item("On Start", "append", 2, "append", "truncate")
    {
    }
};

}

namespace chucho
{

namespace config
{

file_writer_item::file_writer_item(QTreeWidget& tree)
    : pattern_formatter_writer_item(tree, "file")
{
    flush_ = new boolean_editable_item("Flush on Append", true);
    insertChild(0, flush_);
    on_start_ = new on_start_editable_item();
    insertChild(0, on_start_);
    file_name_ = new text_editable_item("File Name", "");
    insertChild(0, file_name_);
}

void file_writer_item::emit_config(std::ostream& stream, std::size_t tabstop)
{
    pattern_formatter_writer_item::emit_config(stream, tabstop++);
    indent(stream, tabstop) << "- file_name: " << file_name_->text(1) << std::endl;
    indent(stream, tabstop) << "- on_start: " << on_start_->text(1) << std::endl;
    indent(stream, tabstop) << "- flush: " << on_start_->text(1) << std::endl;
}

}

}