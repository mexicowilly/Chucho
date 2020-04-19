/*
 * Copyright 2013-2019 Will Mason
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

#if !defined(CHUCHO_KAFKA_WRITER_HPP__)
#define CHUCHO_KAFKA_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>
#include <librdkafka/rdkafka.h>
#include <thread>

namespace chucho
{

class kafka_writer : public message_queue_writer
{
public:
    kafka_writer(const std::string& name,
                 std::unique_ptr<formatter>&& fmt,
                 std::unique_ptr<serializer>&& ser,
                 const std::string& topic,
                 rd_kafka_conf_t* conf);
    ~kafka_writer();

    std::string get_config_value(const std::string& key) const;
    std::string get_topic() const;

protected:
    virtual void flush_impl(const std::vector<std::uint8_t>& blob) override;

private:
    void poller_main();

    rd_kafka_t* producer_;
    rd_kafka_topic_t* topic_;
    std::thread poller_;
    std::atomic_bool should_stop_;
};

}

#endif

