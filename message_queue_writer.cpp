/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/message_queue_writer.hpp>

namespace chucho
{

const std::size_t message_queue_writer::DEFAULT_COALESCE_MAX = 25;

message_queue_writer::message_queue_writer(const std::string& name,
                                           std::unique_ptr<formatter>&& fmt,
                                           std::unique_ptr<serializer>&& ser,
                                           std::unique_ptr<compressor>&& cmp)
    : writer(name, std::move(fmt)),
      serializer_(std::move(ser)),
      compressor_(std::move(cmp)),
      coalesce_max_(DEFAULT_COALESCE_MAX),
      number_coalesced_(0)
{
    if (!serializer_)
        throw std::invalid_argument("The serializer must be set");
}

message_queue_writer::message_queue_writer(const std::string& name,
                                           std::unique_ptr<formatter>&& fmt,
                                           std::unique_ptr<serializer>&& ser,
                                           std::size_t coalesce_max,
                                           std::unique_ptr<compressor>&& cmp)
    : writer(name, std::move(fmt)),
      serializer_(std::move(ser)),
      compressor_(std::move(cmp)),
      coalesce_max_(coalesce_max),
      number_coalesced_(0)
{
    if (!serializer_)
        throw std::invalid_argument("The serializer must be set");
}

message_queue_writer::~message_queue_writer()
{
    if (number_coalesced_ > 0)
        report_warning("The message queue writer is closing with " + std::to_string(number_coalesced_) + " unflushed events");
}

void message_queue_writer::flush()
{
    auto bytes = serializer_->finish_blob();
    if (compressor_)
        bytes = compressor_->compress(bytes);
    flush_impl(bytes);
    number_coalesced_ = 0;
}

void message_queue_writer::write_impl(const event& evt)
{
    serializer_->serialize(evt, *formatter_);
    if (++number_coalesced_ == coalesce_max_)
        flush();
}

}
