/*
 * Copyright 2013-2014 Will Mason
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

#include <chucho/file_writer_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/text_util.hpp>

namespace chucho
{

file_writer_memento::file_writer_memento(configurator& cfg, memento_key_set ks)
    : writer_memento(cfg),
      start_(file_writer::on_start::APPEND),
      flush_(true)
{
    set_status_origin("file_writer_memento");
    handler fn_hnd = [this] (const std::string& name) { file_name_ = validate("file_writer::file_name", name); };
    handler flsh_hnd = [this] (const std::string& val) { flush_ = boolean_value(validate("file_writer::flush", val)); };
    if (ks == memento_key_set::CHUCHO)
    {
        set_handler("file_name", fn_hnd);
        set_handler("on_start", std::bind(&file_writer_memento::set_on_start, this, std::placeholders::_1));
        set_handler("flush", flsh_hnd);
    }
    else if (ks == memento_key_set::LOG4CPLUS)
    {
        set_handler("File", fn_hnd);
        set_handler("Append", [this] (const std::string& val) { set_on_start(boolean_value(validate("file_writer::on_start", val)) ? "append" : "truncate"); });
        set_handler("ImmediateFlush", flsh_hnd);
        start_ = file_writer::on_start::TRUNCATE;
    }
}

void file_writer_memento::set_on_start(const std::string& value)
{
    std::string low = text_util::to_lower(validate("file_writer::on_start", value));
    if (low == "append")
        start_ = file_writer::on_start::APPEND;
    else if (low == "truncate")
        start_ = file_writer::on_start::TRUNCATE;
    else
        throw exception("on_start has an invalid value of " + value);
}

}
