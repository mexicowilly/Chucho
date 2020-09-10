/*
 * Copyright 2013-2020 Will Mason
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

#include <chucho/kafka_writer.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

using namespace std::chrono_literals;

kafka_writer::kafka_writer(const std::string& name,
                           std::unique_ptr<formatter>&& fmt,
                           std::unique_ptr<serializer>&& ser,
                           const std::string& topic,
                           rd_kafka_conf_t* conf)
    : message_queue_writer(name, std::move(fmt), std::move(ser), 1),
      config_(conf),
      should_stop_(false)
{
    try
    {
        char err_msg[1024];
        auto local = rd_kafka_conf_dup(config_);
        producer_ = rd_kafka_new(RD_KAFKA_PRODUCER, local, err_msg, sizeof(err_msg));
        if (producer_ == nullptr)
        {
            rd_kafka_conf_destroy(local);
            throw exception(std::string("Unable to initialize rdkafka: ") + err_msg);
        }
        topic_ = rd_kafka_topic_new(producer_, topic.c_str(), nullptr);
        if (topic_ == nullptr)
        {
            rd_kafka_destroy(producer_);
            throw exception("Unable to create topic '" + topic + "': " + rd_kafka_err2str(rd_kafka_last_error()));
        }
        poller_ = std::thread(&kafka_writer::poller_main, this);
    }
    catch (...)
    {
        rd_kafka_conf_destroy(config_);
        throw;
    }
}

kafka_writer::~kafka_writer()
{
    should_stop_ = true;
    poller_.join();
    rd_kafka_flush(producer_, 5000);
    rd_kafka_topic_destroy(topic_);
    rd_kafka_destroy(producer_);
    rd_kafka_conf_destroy(config_);
}

void kafka_writer::flush_impl(const std::vector<std::uint8_t>& blob)
{
    while (true)
    {
        int rc = rd_kafka_produce(topic_,
                                  RD_KAFKA_PARTITION_UA,
                                  RD_KAFKA_MSG_F_COPY,
                                  const_cast<void*>(reinterpret_cast<const void*>(&blob[0])),
                                  blob.size(),
                                  nullptr,
                                  0,
                                  nullptr);
        if (rc == 0)
        {
            break;
        }
        else if (rc == RD_KAFKA_RESP_ERR__QUEUE_FULL)
        {
            // Give our poller a chance to flush things
            std::this_thread::sleep_for(250ms);
        }
        else
        {
            throw exception(std::string("Kafka error writing to topic '") + rd_kafka_topic_name(topic_) + "': " + rd_kafka_err2str(rd_kafka_last_error()));
        }
    }
}

std::string kafka_writer::get_config_value(const std::string& key) const
{
    char value[1024 * 4];
    std::size_t len = sizeof(value);
    if (rd_kafka_conf_get(config_, key.c_str(), value, &len) == RD_KAFKA_CONF_OK)
        return value;
    throw exception("Unable to retrieve Kafka setting for '" + key + '"');
}

std::string kafka_writer::get_topic() const
{
    return rd_kafka_topic_name(topic_);
}

void kafka_writer::poller_main()
{
    while (!should_stop_)
        rd_kafka_poll(producer_, 500);
}

}
