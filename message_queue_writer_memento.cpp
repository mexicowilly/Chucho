/*
 * Copyright 2013-2017 Will Mason
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

#include <chucho/message_queue_writer_memento.hpp>
#include <chucho/message_queue_writer.hpp>
#include <chucho/move_util.hpp>

namespace chucho
{

message_queue_writer_memento::message_queue_writer_memento(configurator& cfg)
    : writer_memento(cfg),
      coalesce_max_(message_queue_writer::DEFAULT_COALESCE_MAX)
{
    set_status_origin("message_queue_writer_memento");
    cfg.get_security_policy().set_integer("message_queue_writer::coalesce_max", 0, 10000);
    cfg.get_security_policy().set_text("message_queue_writer::coalesce_max(text)", 5);
    set_handler("coalesce_max",
                [this] (const std::string& cm)
                {
                    coalesce_max_ = validate("message_queue_writer::coalesce_max",
                                             std::stoul(validate("message_queue_writer::coalesce_max(text)",
                                                                 cm)));
                });
}

void message_queue_writer_memento::handle(std::unique_ptr<configurable>&& cnf)
{
    auto ser = dynamic_move<serializer>(std::move(cnf));
    if (ser)
    {
        serializer_ = std::move(ser);
    }
    else
    {
        auto cmp = dynamic_move<compressor>(std::move(cnf));
        if (cmp)
            compressor_ = std::move(cmp);
        else
            writer_memento::handle(std::move(cnf));
    }
}

}
