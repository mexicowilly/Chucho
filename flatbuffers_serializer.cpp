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

#include <chucho/flatbuffers_serializer.hpp>
#include <chucho/utf8.hpp>
#include <chucho/logger.hpp>
#include <chucho/host.hpp>
#include "chucho_generated.h"
#include <sstream>
#include <thread>

namespace chucho
{

struct flatbuffers_serializer::handle
{
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<flatb::log_event>> events;
};

flatbuffers_serializer::flatbuffers_serializer()
    : handle_(new handle)
{
}

// Don't remove this. It has to be defined here.
flatbuffers_serializer::~flatbuffers_serializer()
{
}

std::vector<std::uint8_t> flatbuffers_serializer::finish_blob()
{
    handle_->builder.Finish(flatb::Createlog_eventsDirect(handle_->builder,
                                                          &handle_->events,
                                                          host::get_full_name().c_str()));
    auto result = std::vector<std::uint8_t>(handle_->builder.GetBufferPointer(),
                                            handle_->builder.GetBufferPointer() + handle_->builder.GetSize());
    handle_->events.clear();
    handle_->builder.Reset();
    return result;
}

void flatbuffers_serializer::serialize(const event& evt, formatter& fmt)
{
    // Strings cannot be created during the time that the builder is active.
    // So, create them before and then start the builder. Flatbuffers gets very
    // angry if you try to nest them.
    auto msg = handle_->builder.CreateString(utf8::escape_invalid(fmt.format(evt)));
    auto fn = handle_->builder.CreateString(utf8::escape_invalid(evt.get_file_name()));
    auto func = handle_->builder.CreateString(utf8::escape_invalid(evt.get_function_name()));
    auto lgr = handle_->builder.CreateString(utf8::escape_invalid(evt.get_logger()->get_name()));
    auto lvl = handle_->builder.CreateString(utf8::escape_invalid(evt.get_level()->get_name()));
    std::ostringstream tstream;
    tstream << std::this_thread::get_id();
    auto thr = handle_->builder.CreateString(utf8::escape_invalid(tstream.str()));
    flatbuffers::Offset<flatbuffers::String> mrk;
    if (evt.get_marker())
    {
        std::ostringstream mstream;
        mstream << *evt.get_marker();
        mrk = handle_->builder.CreateString(utf8::escape_invalid(mstream.str()));
    }
    flatb::log_eventBuilder eb(handle_->builder);
    eb.add_formatted_message(msg);
    eb.add_seconds_since_epoch(event::clock_type::to_time_t(evt.get_time()));
    eb.add_file_name(fn);
    eb.add_line_number(evt.get_line_number());
    eb.add_function_name(func);
    eb.add_logger(lgr);
    eb.add_level_name(lvl);
    if (!mrk.IsNull())
        eb.add_marker(mrk);
    eb.add_thread(thr);
    handle_->events.emplace_back(eb.Finish());
}

}

