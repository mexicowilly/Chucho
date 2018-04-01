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

#if !defined(CHUCHO_RABBITMQ_WRITER_HPP__)
#define CHUCHO_RABBITMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>
#include <chucho/optional.hpp>
#include <amqp.h>

namespace chucho
{

/**
 * @class rabbitmq_writer rabbitmq_writer.hpp chucho/rabbitmq_writer.hpp
 * A writer that publishes <a href="http://http://www.rabbitmq.com/">RabbitMQ</a> messages.
 * 
 * @ingroup mq writers
 */
class CHUCHO_EXPORT rabbitmq_writer : public message_queue_writer
{
public:
    /**
     * @name Constructors and Destructor
     * @{
    */
    /**
     * Construct a RabbitMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param url the URL to the RabbitMQ instance
     * @param exchange the exchange to which to publish
     * @param routing_key the routing key
     */
    rabbitmq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    const std::string& url,
                    const std::string& exchange,
                    const optional<std::string>& routing_key = optional<std::string>());
    /**
     * Construct a RabbitMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single RabbitMQ message
     * @param url the URL to the RabbitMQ instance
     * @param exchange the exchange to which to publish
     * @param routing_key the routing key
     */
    rabbitmq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::size_t coalesce_max,
                    const std::string& url,
                    const std::string& exchange,
                    const optional<std::string>& routing_key = optional<std::string>());
    /**
     * Construct a RabbitMQ writer.
     * 
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param cmp the compressor
     * @param url the URL to the RabbitMQ instance
     * @param exchange the exchange to which to publish
     * @param routing_key the routing key
     */
    rabbitmq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::unique_ptr<compressor>&& cmp,
                    const std::string& url,
                    const std::string& exchange,
                    const optional<std::string>& routing_key = optional<std::string>());
    /**
     * Construct a RabbitMQ writer.
     *
     * @param name the name of this writer
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single RabbitMQ message
     * @param cmp the compressor
     * @param url the URL to the RabbitMQ instance
     * @param exchange the exchange to which to publish
     * @param routing_key the routing key
     */
    rabbitmq_writer(const std::string& name,
                    std::unique_ptr<formatter>&& fmt,
                    std::unique_ptr<serializer>&& ser,
                    std::size_t coalesce_max,
                    std::unique_ptr<compressor>&& cmp,
                    const std::string& url,
                    const std::string& exchange,
                    const optional<std::string>& routing_key = optional<std::string>());
    /**
     * Destroy a RabbitMQ writer.
     */
    ~rabbitmq_writer();
    /**
     * @}
     */

    /**
     * Return the exchange to which message are published.
     * 
     * @return the exchange
     */
    const std::string& get_exchange() const;
    /**
     * Return the routing key.
     * 
     * @return the the routing key
     */
    const std::string& get_routing_key() const;
    /**
     * Return the routing URL.
     * 
     * @return the the URL
     */
    const std::string& get_url() const;

protected:
    virtual void flush_impl(const std::vector<std::uint8_t>& bytes) override;

private:
    CHUCHO_NO_EXPORT void init();
    CHUCHO_NO_EXPORT void respond(const amqp_rpc_reply_t& rep);

    std::string url_;
    std::string exchange_;
    std::string routing_key_;
    amqp_connection_state_t cxn_;
    amqp_bytes_t exchange_bytes_;
    amqp_bytes_t routing_key_bytes_;
};

inline const std::string& rabbitmq_writer::get_exchange() const
{
    return exchange_;
}

inline const std::string& rabbitmq_writer::get_routing_key() const
{
    return routing_key_;
}

inline const std::string& rabbitmq_writer::get_url() const
{
    return url_;
}

}

#endif
