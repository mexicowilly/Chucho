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

#include "pattern_formatter_writer_item.hpp"
#include "text_editable_item.hpp"

namespace chucho
{

namespace config
{

pattern_formatter_writer_item::pattern_formatter_writer_item(QTreeWidget& tree, const std::string& emit_name)
    : writer_item(tree),
      emit_name_(emit_name)
{
    pattern_formatter_ = new text_editable_item("Pattern Formatter", "%d{%H:%M:%S.%q} %-5p %.36c - %m%n");
    insertChild(0, pattern_formatter_);
}

void pattern_formatter_writer_item::emit_config(std::ostream& stream, std::size_t tabstop)
{
    indent(stream, tabstop++) << "- chucho::" << emit_name_ << "_writer:" << std::endl;
    indent(stream, tabstop++) << "- chucho::pattern_formatter:" << std::endl;
    indent(stream, tabstop) << "- pattern: " << pattern_formatter_->text(1) << std::endl;
    writer_item::emit_config(stream, tabstop);
}

}

}