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

#if !defined(CHUCHO_FORMATTED_MESSAGE_SERIALIZER_HPP__)
#define CHUCHO_FORMATTED_MESSAGE_SERIALIZER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/serializer.hpp>

namespace chucho
{

/**
 * @class formatted_message_serializer formatted_message_serializer.hpp chucho/formatted_message_serializer.hpp
 * A serializer that simply turns the text of a formatted message
 * into a blob. This class is meant for a simple case where the full context
 * of the log event is not necessary. If you need all the event data
 * for your message, please consider using the @ref protobuf_serializer.
 *
 * The events are concatenated together into a single string with newlines
 * separating the events.
 * 
 * @ingroup mq
 */
class CHUCHO_EXPORT formatted_message_serializer : public serializer
{
public:
    virtual std::vector<std::uint8_t> finish_blob() override;
    /**
     * Format the message of the event using the given @ref formatter and
     * put copy those bytes into the returned blob. Each message is
     * separated by a platform-specific new line.
     * 
     * @param evt the event
     * @param fmt the formatter
     */
    virtual void serialize(const event& evt, formatter& fmt) override;

private:
    std::string events_;
};

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
