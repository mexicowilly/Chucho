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

#if !defined(CHUCHO_ZEROMQ_WRITER_HPP__)
#define CHUCHO_ZEROMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>

namespace chucho
{

/**
 * @class zeromq_writer zeromq_writer.hpp chucho/zeromq_writer.hpp
 * A writer that publishes <a href="http://zeromq.org/">ZeroMQ</a> messages.
 * To read log events published to the given endpoint, you would connect
 * a ZeroMQ socket of type ZMQ_SUB. Additionally, a "topic" can be used
 * by providing a prefix in the writer's constructor. If a prefix is
 * provided, then each event will be published as a two-part message.
 * 
 * @ingroup mq writers
 */
class CHUCHO_EXPORT zeromq_writer : public message_queue_writer
{
public:
    /**
     * @name Constructors and Destructor
     * @{
     */
    /**
     * Construct a ZeroMQ writer.
     * 
     * @param fmt the formatter
     * @param ser the serializer
     * @param endpoint the ZeroMQ endpoint to which to bind the publishing socket
     * @param prefix the message prefix, which will be used as the "topic". If a
     * prefix is provided, then each event will be published as a two-part message,
     * first prefix, then event.
     */
    zeromq_writer(std::shared_ptr<formatter> fmt,
                  std::shared_ptr<serializer> ser,
                  const std::string& endpoint,
                  const std::vector<std::uint8_t>& prefix = std::vector<std::uint8_t>());
    /**
     * Construct a ZeroMQ writer.
     *
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single ZermoMQ message
     * @param endpoint the ZeroMQ endpoint to which to bind the publishing socket
     * @param prefix the message prefix, which will be used as the "topic". If a
     * prefix is provided, then each event will be published as a two-part message,
     * first prefix, then event.
     */
    zeromq_writer(std::shared_ptr<formatter> fmt,
                  std::shared_ptr<serializer> ser,
                  std::size_t coalesce_max,
                  const std::string& endpoint,
                  const std::vector<std::uint8_t>& prefix = std::vector<std::uint8_t>());
    /**
     * Construct a ZeroMQ writer.
     * 
     * @param fmt the formatter
     * @param ser the serializer
     * @param cmp the compressor
     * @param endpoint the ZeroMQ endpoint to which to bind the publishing socket
     * @param prefix the message prefix, which will be used as the "topic". If a
     * prefix is provided, then each event will be published as a two-part message,
     * first prefix, then event.
     */
    zeromq_writer(std::shared_ptr<formatter> fmt,
                  std::shared_ptr<serializer> ser,
                  std::shared_ptr<compressor> cmp,
                  const std::string& endpoint,
                  const std::vector<std::uint8_t>& prefix = std::vector<std::uint8_t>());
    /**
     * Construct a ZeroMQ writer.
     *
     * @param fmt the formatter
     * @param ser the serializer
     * @param coalesce_max the maximum number of events to write in a single ZermoMQ message
     * @param cmp the compressor
     * @param endpoint the ZeroMQ endpoint to which to bind the publishing socket
     * @param prefix the message prefix, which will be used as the "topic". If a
     * prefix is provided, then each event will be published as a two-part message,
     * first prefix, then event.
     */
    zeromq_writer(std::shared_ptr<formatter> fmt,
                  std::shared_ptr<serializer> ser,
                  std::size_t coalesce_max,
                  std::shared_ptr<compressor> cmp,
                  const std::string& endpoint,
                  const std::vector<std::uint8_t>& prefix = std::vector<std::uint8_t>());
    /**
     * Destroy a ZeroMQ writer.
     */
    ~zeromq_writer();
    /**
     * @}
     */

    /**
     * Return the endpoint to which the socket is connected.
     * 
     * @return the endpoint
     */
    const std::string& get_endpoint() const;
    /**
     * Return the prefix, or message topic, that is being used.
     * 
     * @return the prefix
     */
    const std::vector<std::uint8_t>& get_prefix() const;

protected:
    virtual void flush_impl(const std::vector<std::uint8_t>& bytes) override;

private:
    CHUCHO_NO_EXPORT void init();

    std::string endpoint_;
    std::vector<std::uint8_t> prefix_;
    void* socket_;
};

inline const std::string& zeromq_writer::get_endpoint() const
{
    return endpoint_;
}

inline const std::vector<std::uint8_t>& zeromq_writer::get_prefix() const
{
    return prefix_;
}

}

#endif
