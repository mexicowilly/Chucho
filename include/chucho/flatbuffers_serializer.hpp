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

#if !defined(CHUCHO_FLATBUFFERS_SERIALIZER_HPP__)
#define CHUCHO_FLATBUFFERS_SERIALIZER_HPP__

#include <chucho/serializer.hpp>

namespace chucho
{

/**
 * @class flatbuffers_serializer flatbuffers_serializer.hpp chucho/flatbuffers_serializer.hpp
 * A @ref serializer that creates a <a href="https://developers.google.com/protocol-buffers/?hl=en">flatbuffers</a>
 * message. The definintion of the message is in the file @c serialization/chucho.proto.
 * All of the event data, including the formatted message, will appear in the
 * resulting message. If you only need the formatted message, then please consider
 * using the @ref formatted_message_serializer instead.
 * 
 * @ingroup mq
 */
class CHUCHO_EXPORT flatbuffers_serializer : public serializer
{
public:
    /**
     * @name Constructor and Destructor
     * @{
     */
    /**
     * Make a flatbuffers serializer.
     */
    flatbuffers_serializer();
    /**
     * Destroy a flatbuffers serializer.
     */
    virtual ~flatbuffers_serializer();
    /**
     * @}
     */

    /**
     * Return a blobified flatbuffers message.
     *
     * @return the flatbuffers message
     */
    virtual std::vector<std::uint8_t> finish_blob() override;
    /**
     * Serialize the event into a flatbuffers message.
     * 
     * @param evt the event
     * @param fmt the formatter
     */
    virtual void serialize(const event& evt, std::shared_ptr<formatter> fmt) override;

private:
    struct handle;

    std::unique_ptr<handle> handle_;
};

}

#endif
