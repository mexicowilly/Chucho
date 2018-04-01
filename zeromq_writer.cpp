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

#include <chucho/zeromq_writer.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/exception.hpp>
#include <thread>
#include <cstring>
#include <zmq.h>

namespace
{

void* get_zmq_context()
{
    static void* ctx;
    static std::once_flag once;

    std::call_once(once, [&] () { ctx = zmq_ctx_new();
                                  chucho::garbage_cleaner::get().add([&] () { zmq_ctx_destroy(ctx); }); });
    return ctx;
}

}

namespace chucho
{

zeromq_writer::zeromq_writer(const std::string& name,
                             std::unique_ptr<formatter>&& fmt,
                             std::unique_ptr<serializer>&& ser,
                             const std::string& endpoint,
                             const std::vector<std::uint8_t>& prefix)
    : message_queue_writer(name, std::move(fmt), std::move(ser)),
      endpoint_(endpoint),
      prefix_(prefix)
{
    init();
}

zeromq_writer::zeromq_writer(const std::string& name,
                             std::unique_ptr<formatter>&& fmt,
                             std::unique_ptr<serializer>&& ser,
                             std::size_t coalesce_max,
                             const std::string& endpoint,
                             const std::vector<std::uint8_t>& prefix)
    : message_queue_writer(name, std::move(fmt), std::move(ser), coalesce_max),
      endpoint_(endpoint),
      prefix_(prefix)
{
    init();
}

zeromq_writer::zeromq_writer(const std::string& name,
                             std::unique_ptr<formatter>&& fmt,
                             std::unique_ptr<serializer>&& ser,
                             std::unique_ptr<compressor>&& cmp,
                             const std::string& endpoint,
                             const std::vector<std::uint8_t>& prefix)
    : message_queue_writer(name, std::move(fmt), std::move(ser), std::move(cmp)),
      endpoint_(endpoint),
      prefix_(prefix)
{
    init();
}

zeromq_writer::zeromq_writer(const std::string& name,
                             std::unique_ptr<formatter>&& fmt,
                             std::unique_ptr<serializer>&& ser,
                             std::size_t coalesce_max,
                             std::unique_ptr<compressor>&& cmp,
                             const std::string& endpoint,
                             const std::vector<std::uint8_t>& prefix)
    : message_queue_writer(name, std::move(fmt), std::move(ser), coalesce_max, std::move(cmp)),
      endpoint_(endpoint),
      prefix_(prefix)
{
    init();
}

zeromq_writer::~zeromq_writer()
{
    zmq_close(socket_);
}

void zeromq_writer::flush_impl(const std::vector<std::uint8_t>& bytes)
{
    if (socket_ != nullptr)
    {
        int rc;
        if (!prefix_.empty())
        {
            zmq_msg_t pre;
            zmq_msg_init_size(&pre, prefix_.size());
            std::memcpy(zmq_msg_data(&pre), &prefix_[0], prefix_.size());
            rc = zmq_msg_send(&pre, socket_, ZMQ_SNDMORE);
            if (rc != prefix_.size())
            {
                zmq_msg_close(&pre);
                throw exception(std::string("Error sending zeromq message prefix: ") + zmq_strerror(rc));
            }
        }
        zmq_msg_t msg;
        zmq_msg_init_size(&msg, bytes.size());
        std::memcpy(zmq_msg_data(&msg), &bytes[0], bytes.size());
        rc = zmq_msg_send(&msg, socket_, 0);
        if (rc != bytes.size())
        {
            zmq_msg_close(&msg);
            throw exception(std::string("Error sending zeromq message: ") + zmq_strerror(rc));
        }
    }
}

void zeromq_writer::init()
{
    socket_ = zmq_socket(get_zmq_context(), ZMQ_PUB);
    if (socket_ == nullptr)
    {
        throw exception("The zeromq socket could not be created because there are too many zeromq sockets open");
    }
    else
    {
        int rc = zmq_bind(socket_, endpoint_.c_str());
        if (rc != 0)
            throw exception("Could not bind to zeromq endpoint " + endpoint_ + ": " + zmq_strerror(rc));
    }
}

}
