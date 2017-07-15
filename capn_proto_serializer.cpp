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

std::vector<std::uint8_t> capn_proto_serializer::serialize(const event& evt,
                                                           std::shared_ptr<formatter> fmt)
{
    ::capnp::MallocMessageBuilder message;
    capnp::Event::Builder cevt = message.initRoot<capnp::Event>();
    cevt.setFormattedMessage(utf8::escape_invalid(fmt->format(evt)));
    cevt.setSecondsSinceEpoch(event::clock_type::to_time_t(evt.get_time()));
    cevt.setFileName(utf8::escape_invalid(evt.get_file_name()));
    cevt.setLineNumber(evt.get_line_number());
    cevt.setFunctionName(utf8::escape_invalid(evt.get_function_name()));
    cevt.setLogger(utf8::escape_invalid(evt.get_logger()->get_name()));
    cevt.setLevelName(utf8::escape_invalid(evt.get_level()->get_name()));
    if (evt.get_marker())
    {
        std::ostringstream mstream;
        mstream << *evt.get_marker();
        cevt.setMarker(utf8::escape_invalid(mstream.str()));
    }
    std::ostringstream tstream;
    tstream << std::this_thread::get_id();
    cevt.setThread(utf8::escape_invalid(tstream.str()));
    auto words = ::capnp::messageToFlatArray(message);
    auto bytes = words.asBytes();
    return std::vector<std::uint8_t>(bytes.begin(), bytes.end());
}

}
