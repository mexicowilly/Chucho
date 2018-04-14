/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/cloudwatch_writer_memento.hpp>
#include <chucho/cloudwatch_writer.hpp>

namespace chucho
{

cloudwatch_writer_memento::cloudwatch_writer_memento(configurator& cfg)
    : writer_memento(cfg)
{
    set_status_origin("file_writer_memento");
    set_default_name(typeid(cloudwatch_writer));
    cfg.get_security_policy().set_text("cloudwatch_writer::log_group", 512);
    cfg.get_security_policy().set_text("cloudwatch_writer::log_stream", 512);
    cfg.get_security_policy().set_integer("cloudwatch_writer::batch_size", 1, 10000);
    cfg.get_security_policy().set_text("cloudwatch_writer::batch_size(text)", 5);
    set_handler("log_group", [this] (const std::string& s) { log_group_ = validate("cloudwatch_writer::log_group", s); });
    set_handler("log_stream", [this] (const std::string& s) { log_stream_ = validate("cloudwatch_writer::log_stream", s); });
    set_handler("region", [this] (const std::string& s) { region_ = validate("cloudwatch_writer::region", s); });
    set_handler("batch_size", [this] (const std::string& s) { batch_size_ = validate("cloudwatch_writer::batch_size",
                                                                                     std::stoul(validate("cloudwatch_writer::batch_size(text)", s))); });
}

}
