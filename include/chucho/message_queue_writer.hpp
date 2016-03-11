/*
 * Copyright 2013-2016 Will Mason
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

#if !defined(CHUCHO_MESSAGE_QUEUE_WRITER_HPP__)
#define CHUCHO_MESSAGE_QUEUE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <chucho/serializer.hpp>
#include <chucho/compressor.hpp>

namespace chucho
{

/**
 * @class message_queue_writer message_queue_writer.hpp chucho/message_queue_writer.hpp
 * A base class for writing to a message queue. The principal
 * service provided by this base class is to require the presence
 * of a @ref serializer and to provide access to it.
 * 
 * Message queues treat messages as blobs. The @ref serializer is
 * responsible for converting the @ref event into a blob.
 * 
 * @ingroup mq writers
 */
class CHUCHO_EXPORT message_queue_writer : public writer
{
public:
    /**
     * Return the compressor.
     * 
     * @return the compressor
     */
    std::shared_ptr<compressor> get_compressor() const;
    /**
     * Return the serializer.
     * 
     * @return the serializer
     */
    std::shared_ptr<serializer> get_serializer() const;

protected:
    /**
     * @name Constructor
     * @{
     */
    /**
     * Construct a message queue writer.
     * 
     * @param fmt the formatter
     * @param ser the serializer
     * @param cmp the compressor
     */
    message_queue_writer(std::shared_ptr<formatter> fmt,
                         std::shared_ptr<serializer> ser,
                         std::shared_ptr<compressor> cmp = std::shared_ptr<compressor>());
    /**
     * @}
     */
    /**
     * Convenience function to serialize and compress
     * 
     * @param evt the event to send
     * @return a vector of serialized and compressed bytes
     */
    std::vector<std::uint8_t> serialize_and_compress(const event& evt);

    /**
     * The serializer
     */
    std::shared_ptr<serializer> serializer_;
    /**
     * The compressor
     */
     std::shared_ptr<compressor> compressor_;
};

inline std::shared_ptr<compressor> message_queue_writer::get_compressor() const
{
    return compressor_;
}

inline std::shared_ptr<serializer> message_queue_writer::get_serializer() const
{
    return serializer_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
