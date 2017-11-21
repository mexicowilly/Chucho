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

#if !defined(CHUCHO_MESSAGE_QUEUE_WRITER_HPP__)
#define CHUCHO_MESSAGE_QUEUE_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <chucho/serializer.hpp>
#include <chucho/compressor.hpp>
#include <atomic>

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
     * The number of log events that will be coalsced into a
     * single message if no other value is specified.
     *
     * The value is 25.
     */
    static const std::size_t DEFAULT_COALESCE_MAX;

    virtual void flush() override;
    /**
     * Return the maximum number of events that can appear
     * in a single message queue message.
     *
     * @return the maximum
     */
    std::size_t get_coalesce_max() const;
    /**
     * Return the compressor.
     * 
     * @return the compressor
     */
    std::shared_ptr<compressor> get_compressor() const;
    /**
     * Get the current number of events that have been added
     * to the outgoing message but not yet sent.
     *
     * @return the current number
     */
    std::size_t get_number_coalesced() const;
    /**
     * Return the serializer.
     * 
     * @return the serializer
     */
    std::shared_ptr<serializer> get_serializer() const;
    /**
     * Set the maximum number of events that can be added to
     * an outgoing message before it is flushed to the message
     * queue.
     *
     * @param num the maximum
     */
    void set_coalesce_max(std::size_t num);

protected:
    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct a message queue writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param cmp the compressor
     */
    message_queue_writer(const std::string& name,
                         std::shared_ptr<formatter> fmt,
                         std::shared_ptr<serializer> ser,
                         std::shared_ptr<compressor> cmp = std::shared_ptr<compressor>());
    /**
     * Construct a message queue writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to collect before sending
     * @param cmp the compressor
     */
    message_queue_writer(const std::string& name,
                         std::shared_ptr<formatter> fmt,
                         std::shared_ptr<serializer> ser,
                         std::size_t coalesce_max,
                         std::shared_ptr<compressor> cmp = std::shared_ptr<compressor>());
    /**
     * Destroy the writer.
     */
    virtual ~message_queue_writer();
    /**
     * @}
     */
    /**
     * Write the blob to the message queue.
     *
     * @param blob the bytes to write
     */
    virtual void flush_impl(const std::vector<std::uint8_t>& blob) = 0;
    virtual void write_impl(const event& evt) override;
    /**
     * The serializer
     */
    std::shared_ptr<serializer> serializer_;
    /**
     * The compressor
     */
    std::shared_ptr<compressor> compressor_;
    /**
     * The maximum number of events to coalesce.
     */
    std::atomic<std::size_t> coalesce_max_;
    /**
     * The current number of coalesced events.
     */
    std::atomic<std::size_t> number_coalesced_;
};

inline std::size_t message_queue_writer::get_coalesce_max() const
{
    return coalesce_max_;
}

inline std::shared_ptr<compressor> message_queue_writer::get_compressor() const
{
    return compressor_;
}

inline std::size_t message_queue_writer::get_number_coalesced() const
{
    return number_coalesced_;
}

inline std::shared_ptr<serializer> message_queue_writer::get_serializer() const
{
    return serializer_;
}

inline void message_queue_writer::set_coalesce_max(std::size_t num)
{
    coalesce_max_ = num;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
