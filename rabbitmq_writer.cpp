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

#include <chucho/rabbitmq_writer.hpp>
#include <chucho/exception.hpp>
#include <amqp_tcp_socket.h>
#include <amqp_ssl_socket.h>
#include <sstream>

namespace
{

const amqp_channel_t CHUCHO_CHANNEL = 1;

}

namespace chucho
{

rabbitmq_writer::rabbitmq_writer(const std::string& name,
                                 std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 const std::string& url,
                                 const std::string& exchange,
                                 const optional<std::string>& routing_key)
    : message_queue_writer(name, fmt, ser),
      url_(url),
      exchange_(exchange),
      routing_key_(routing_key ? *routing_key : ""),
      cxn_(amqp_new_connection())
{
    init();
}

rabbitmq_writer::rabbitmq_writer(const std::string& name,
                                 std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 std::size_t coalesce_max,
                                 const std::string& url,
                                 const std::string& exchange,
                                 const optional<std::string>& routing_key)
    : message_queue_writer(name, fmt, ser, coalesce_max),
      url_(url),
      exchange_(exchange),
      routing_key_(routing_key ? *routing_key : ""),
      cxn_(amqp_new_connection())
{
    init();
}

rabbitmq_writer::rabbitmq_writer(const std::string& name,
                                 std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 std::shared_ptr<compressor> cmp,
                                 const std::string& url,
                                 const std::string& exchange,
                                 const optional<std::string>& routing_key)
    : message_queue_writer(name, fmt, ser, cmp),
      url_(url),
      exchange_(exchange),
      routing_key_(routing_key ? *routing_key : ""),
      cxn_(amqp_new_connection())
{
    init();
}

rabbitmq_writer::rabbitmq_writer(const std::string& name,
                                 std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 std::size_t coalesce_max,
                                 std::shared_ptr<compressor> cmp,
                                 const std::string& url,
                                 const std::string& exchange,
                                 const optional<std::string>& routing_key)
    : message_queue_writer(name, fmt, ser, coalesce_max, cmp),
      url_(url),
      exchange_(exchange),
      routing_key_(routing_key ? *routing_key : ""),
      cxn_(amqp_new_connection())
{
    init();
}
rabbitmq_writer::~rabbitmq_writer()
{
    amqp_channel_close(cxn_, CHUCHO_CHANNEL, AMQP_REPLY_SUCCESS);
    amqp_connection_close(cxn_, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(cxn_);
}

void rabbitmq_writer::flush_impl(const std::vector<std::uint8_t>& bytes)
{
    amqp_bytes_t msg;
    msg.bytes = const_cast<std::uint8_t*>(&bytes[0]);
    msg.len = bytes.size();
    int rc = amqp_basic_publish(cxn_,
                                CHUCHO_CHANNEL,
                                exchange_bytes_,
                                routing_key_bytes_,
                                0,
                                0,
                                nullptr,
                                msg);
    if (rc != AMQP_STATUS_OK)
        throw exception(std::string("Unable to send RabbitMQ message: ") + amqp_error_string2(rc));
}

void rabbitmq_writer::init()
{
    if (cxn_ == 0)
        throw exception("Unable to create a RabbitMQ connection object");
    try
    {
        std::vector<char> chars(url_.c_str(), url_.c_str() + url_.length() + 1);
        amqp_connection_info info;
        int rc = amqp_parse_url(&chars[0], &info);
        if (rc != AMQP_STATUS_OK)
            throw exception("Invalid RabbitMQ URL: " + url_);
        if (info.ssl)
            throw exception("Chucho does not support RabbitMQ connections using SSL");
        amqp_socket_t* sock = amqp_tcp_socket_new(cxn_);
        if (sock == nullptr)
            throw exception("Unable to create socket");
        rc = amqp_socket_open(sock, info.host, info.port);
        if (rc != AMQP_STATUS_OK)
            throw exception(std::string("Error connecting to RabbitMQ host: ") + amqp_error_string2(rc));
        amqp_rpc_reply_t rep = amqp_login(cxn_,
                                          info.vhost,
                                          0,
                                          AMQP_DEFAULT_FRAME_SIZE,
                                          0,
                                          AMQP_SASL_METHOD_PLAIN,
                                          info.user,
                                          info.password);
        respond(rep);
        amqp_channel_open(cxn_, CHUCHO_CHANNEL);
        rep = amqp_get_rpc_reply(cxn_);
        respond(rep);
        exchange_bytes_ = amqp_cstring_bytes(exchange_.c_str());
        routing_key_bytes_ = amqp_cstring_bytes(routing_key_.c_str());
    }
    catch (...)
    {
        amqp_channel_close(cxn_, CHUCHO_CHANNEL, AMQP_REPLY_SUCCESS);
        amqp_connection_close(cxn_, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(cxn_);
        throw;
    }
}

void rabbitmq_writer::respond(const amqp_rpc_reply_t& rep)
{
    if (rep.reply_type == AMQP_RESPONSE_NORMAL)
        return;
    std::ostringstream stream;
    stream << "The RabbitMQ broker reports an error: ";
    if (rep.reply_type == AMQP_RESPONSE_NONE)
    {
        stream << "Missing reply type";
    }
    else if (rep.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION)
    {
        stream << amqp_error_string2(rep.library_error);
    }
    else if (rep.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION)
    {
        if (rep.reply.id == AMQP_CHANNEL_CLOSE_METHOD)
        {
            amqp_channel_close_t* ch = reinterpret_cast<amqp_channel_close_t*>(rep.reply.decoded);
            stream << "Channel: (" << ch->reply_code << ") " <<
                std::string(reinterpret_cast<char*>(ch->reply_text.bytes), ch->reply_text.len);
        }
        else if (rep.reply.id == AMQP_CONNECTION_CLOSE_METHOD)
        {
            amqp_connection_close_t* cn = reinterpret_cast<amqp_connection_close_t*>(rep.reply.decoded);
            stream << "Connection: (" << cn->reply_code << ") " <<
                std::string(reinterpret_cast<char*>(cn->reply_text.bytes), cn->reply_text.len);
        }
        else
        {
            stream << "Unknown method: " << rep.reply.id;
        }
    }
    throw exception(stream.str());
}

}
