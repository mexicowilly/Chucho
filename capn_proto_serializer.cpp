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

#include <chucho/capn_proto_serializer.hpp>
#include <chucho/utf8.hpp>
#include <chucho/logger.hpp>
#include <capnp/serialize.h>
#include "chucho.capnp.h"
#include <cstring>
#include <sstream>
#include <thread>

namespace chucho
{

struct capn_proto_serializer::handle
{
    std::vector<std::pair<event, std::string>> events;
};

capn_proto_serializer::capn_proto_serializer()
    : handle_(new handle)
{
}

capn_proto_serializer::~capn_proto_serializer()
{
}

std::vector<std::uint8_t> capn_proto_serializer::finish_blob()
{
    ::capnp::MallocMessageBuilder message;
    capnp::Events::Builder events = message.initRoot<capnp::Events>();
    auto cevts = events.initEvents(handle_->events.size());
    for (auto i = 0; i < handle_->events.size(); i++)
    {
        capnp::Event::Builder evt = cevts[i];
        const auto& chevt = handle_->events[i].first;
        evt.setFormattedMessage(handle_->events[i].second);
        evt.setSecondsSinceEpoch(event::clock_type::to_time_t(chevt.get_time()));
        evt.setFileName(utf8::escape_invalid(chevt.get_file_name()));
        evt.setLineNumber(chevt.get_line_number());
        evt.setFunctionName(utf8::escape_invalid(chevt.get_function_name()));
        evt.setLogger(utf8::escape_invalid(chevt.get_logger()->get_name()));
        evt.setLevelName(utf8::escape_invalid(chevt.get_level()->get_name()));
        if (chevt.get_marker())
        {
            std::ostringstream mstream;
            mstream << *chevt.get_marker();
            evt.setMarker(utf8::escape_invalid(mstream.str()));
        }
        std::ostringstream tstream;
        tstream << std::this_thread::get_id();
        evt.setThread(utf8::escape_invalid(tstream.str()));

    }
    handle_->events.clear();
    auto words = ::capnp::messageToFlatArray(message);
    auto bytes = words.asBytes();
    return std::vector<std::uint8_t>(bytes.begin(), bytes.end());
}

void capn_proto_serializer::serialize(const event& evt, std::shared_ptr<formatter> fmt)
{
    handle_->events.push_back(std::make_pair(evt, utf8::escape_invalid(fmt->format(evt))));
}

}
