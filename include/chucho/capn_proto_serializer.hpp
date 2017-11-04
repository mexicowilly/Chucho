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

#if !defined(CHUCHO_CAPN_PROTO_SERIALIZER_HPP__)
#define CHUCHO_CAPN_PROTO_SERIALIZER_HPP__

#include <chucho/serializer.hpp>

namespace chucho
{

/**
 * @class capn_proto_serializer capn_proto_serializer.hpp chucho/capn_proto_serializer.hpp
 * A @ref serializer that creates a <a href="https://capnproto.org/">Cap'n Proto</a>
 * message. The definintion of the message is in the file @c serialization/chucho.capnp.
 * All of the event data, including the formatted message, will appear in the
 * resulting message. If you only need the formatted message, then please consider
 * using the @ref formatted_message_serializer instead.
 * 
 * @ingroup mq
 */
class CHUCHO_EXPORT capn_proto_serializer : public serializer
{
public:
    virtual std::vector<std::uint8_t> finish_blob() override;
    virtual void serialize(const event& evt, std::shared_ptr<formatter> fmt) override;

private:
    struct event_store
    {
        event_store(const chucho::event& e, const std::string& fm, const std::string& thr)
            : evt(e), formatted_message(fm), thread(thr) { }

        chucho::event evt;
        std::string formatted_message;
        std::string thread;
    };

    std::vector<event_store> events_;
};

}

#endif
