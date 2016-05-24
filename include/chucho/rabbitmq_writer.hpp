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

#if !defined(CHUCHO_RABBITMQ_WRITER_HPP__)
#define CHUCHO_RABBITMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>
#include <chucho/optional.hpp>
#include <amqp.h>

namespace chucho
{

class CHUCHO_EXPORT rabbitmq_writer : public message_queue_writer
{
public:
    rabbitmq_writer(std::shared_ptr<formatter> fmt,
                    std::shared_ptr<serializer> ser,
                    const std::string& host,
                    std::uint16_t port,
                    const std::string& user,
                    const std::string& password,
                    const std::string& exchange,
                    const optional<std::string>& virtual_host = optional<std::string>(),
                    const optional<std::string>& routing_key = optional<std::string>());
    rabbitmq_writer(std::shared_ptr<formatter> fmt,
                    std::shared_ptr<serializer> ser,
                    std::shared_ptr<compressor> cmp,
                    const std::string& host,
                    std::uint16_t port,
                    const std::string& user,
                    const std::string& password,
                    const std::string& exchange,
                    const optional<std::string>& virtual_host = optional<std::string>(),
                    const optional<std::string>& routing_key = optional<std::string>());
    ~rabbitmq_writer();

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void init();
    CHUCHO_NO_EXPORT void respond(const amqp_rpc_reply_t& rep);

    std::string host_;
    std::uint16_t port_;
    std::string user_;
    std::string password_;
    std::string exchange_;
    std::string vhost_;
    std::string routing_key_;
    amqp_connection_state_t cxn_;
    amqp_channel_t channel_;
};

}

#endif