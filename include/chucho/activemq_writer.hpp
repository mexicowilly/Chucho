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

#if !defined(CHUCHO_ACTIVEMQ_WRITER_HPP__)
#define CHUCHO_ACTIVEMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/ExceptionListener.h>

namespace chucho
{

/**
 * @class activemq_writer activemq_writer.hpp chucho/activemq_writer.hpp
 * A writer that publishes <a href="http://activemq.apache.org/">ActiveMQ</a> messages.
 * You may publish either to a topic or a queue.
 * 
 * @ingroup mq writers
 */
class CHUCHO_EXPORT activemq_writer : public message_queue_writer, public cms::ExceptionListener
{
public:
    /**
     * Whether messages will be published to a topic or a queue.
     */
    enum class consumer_type
    {
        QUEUE,  /**< Publish to a queue */
        TOPIC   /**< Publish to a topic */
    };

    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct an ActiveMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param broker the URI of the ActiveMQ broker
     * @param tp whether to publish to a topic or queue
     * @param topic_or_queue the name of the topic or queue
     */
    activemq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    /**
     * Construct an ActiveMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single ActiveMQ message
     * @param broker the URI of the ActiveMQ broker
     * @param tp whether to publish to a topic or queue
     * @param topic_or_queue the name of the topic or queue
     */
    activemq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::size_t coalesce_max,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    /**
     * Construct an ActiveMQ writer.
     * 
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param cmp the compressor
     * @param broker the URI of the ActiveMQ broker
     * @param tp whether to publish to a topic or queue
     * @param topic_or_queue the name of the topic or queue
     */
    activemq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::unique_ptr<compressor>&& cmp,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    /**
     * Construct an ActiveMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single ActiveMQ message
     * @param cmp the compressor
     * @param broker the URI of the ActiveMQ broker
     * @param tp whether to publish to a topic or queue
     * @param topic_or_queue the name of the topic or queue
     */
    activemq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::size_t coalesce_max,
                    std::unique_ptr<compressor>&& cmp,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    /**
     * Destroy the writer.
     */
    ~activemq_writer();
    /**
     * @}
     */

    /**
     * Return the URI of the ActiveMQ broker.
     * 
     * @return the broker URI
     */
    const std::string& get_broker() const;
    /**
     * Return whether this writer is publishing to a topic or queue.
     *
     * @return the consumer type
     */
    consumer_type get_consumer_type() const;
    /**
     * Return the name of the topic or queue.
     * 
     * @return the topic or queue name
     */
    const std::string& get_topic_or_queue() const;
    /**
     * You don't care what this does.
     */
    virtual void onException(const cms::CMSException& e) override;

protected:
    virtual void flush_impl(const std::vector<std::uint8_t>& bytes) override;

private:
    CHUCHO_NO_EXPORT void init();

    std::string broker_;
    consumer_type type_;
    std::string topic_or_queue_;
    cms::Connection* connection_;
    cms::Session* session_;
    cms::Destination* destination_;
    cms::MessageProducer* producer_;
};

inline const std::string& activemq_writer::get_broker() const
{
    return broker_;
}

inline activemq_writer::consumer_type activemq_writer::get_consumer_type() const
{
    return type_;
}

inline const std::string& activemq_writer::get_topic_or_queue() const
{
    return topic_or_queue_;
}

}

#endif
