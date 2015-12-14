/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_PROTOBUF_SERIALIZER_HPP__)
#define CHUCHO_PROTOBUF_SERIALIZER_HPP__

#include <chucho/serializer.hpp>

namespace chucho
{

/**
 * @class protobuf_serializer protobuf_serializer.hpp chucho/protobuf_serializer.hpp
 * A @ref serializer that creates a <a href="https://developers.google.com/protocol-buffers/?hl=en">protobuf</a>
 * message. The definintion of the message is in the file @c protobuf/chucho.proto.
 * All of the event data, including the formatted message, will appear in the
 * resulting message. If you only need the formatted message, then please consider
 * using the @ref formatted_message_serializer instead.
 * 
 * @ingroup mq
 */
class CHUCHO_EXPORT protobuf_serializer : public serializer
{
public:
    /**
     * Serialize the event into a protobuf message.
     * 
     * @param evt the event
     * @param fmt the formatter
     * @return the blob containing only the formatted message
     */
    virtual std::vector<std::uint8_t> serialize(const event& evt,
                                                std::shared_ptr<formatter> fmt) override;
};

}

#endif