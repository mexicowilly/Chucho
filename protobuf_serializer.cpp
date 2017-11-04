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

#include <chucho/protobuf_serializer.hpp>
#include <chucho/utf8.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include "chucho.pb.h"
#include <cstring>
#include <sstream>
#include <thread>

namespace chucho
{

struct protobuf_serializer::handle
{
    proto::log_events events;
};

protobuf_serializer::protobuf_serializer()
    : handle_(new handle)
{
}

// Don't remove this. It has to be defined here.
protobuf_serializer::~protobuf_serializer()
{
}

std::vector<std::uint8_t> protobuf_serializer::finish_blob()
{
    std::vector<std::uint8_t> result;
    handle_->events.set_host_name(host::get_full_name());
    std::string tmp = handle_->events.SerializeAsString();
    handle_->events.clear_events();
    return std::vector<std::uint8_t>(tmp.begin(), tmp.end());
}

void protobuf_serializer::serialize(const event& evt, std::shared_ptr<formatter> fmt)
{
    proto::log_event& pevt(*handle_->events.add_events());
    pevt.set_formatted_message(utf8::escape_invalid(fmt->format(evt)));
    pevt.set_seconds_since_epoch(event::clock_type::to_time_t(evt.get_time()));
    pevt.set_file_name(utf8::escape_invalid(evt.get_file_name()));
    pevt.set_line_number(evt.get_line_number());
    pevt.set_function_name(utf8::escape_invalid(evt.get_function_name()));
    pevt.set_logger(utf8::escape_invalid(evt.get_logger()->get_name()));
    pevt.set_level_name(utf8::escape_invalid(evt.get_level()->get_name()));
    if (evt.get_marker())
    {
        std::ostringstream mstream;
        mstream << *evt.get_marker();
        pevt.set_marker(utf8::escape_invalid(mstream.str()));
    }
    std::ostringstream tstream;
    tstream << std::this_thread::get_id();
    pevt.set_thread(utf8::escape_invalid(tstream.str()));
}

}
