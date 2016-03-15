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

#if !defined(CHUCHO_ACTIVEMQ_WRITER_HPP__)
#define CHUCHO_ACTIVEMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/ExceptionListener.h>

namespace chucho
{

class activemq_writer : public message_queue_writer, public cms::ExceptionListener
{
public:
    enum class consumer_type
    {
        QUEUE,
        TOPIC
    };

    activemq_writer(std::shared_ptr<formatter> fmt,
                    std::shared_ptr<serializer> ser,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    activemq_writer(std::shared_ptr<formatter> fmt,
                    std::shared_ptr<serializer> ser,
                    std::shared_ptr<compressor> cmp,
                    const std::string& broker,
                    consumer_type tp,
                    const std::string& topic_or_queue);
    ~activemq_writer();

    const std::string& get_broker() const;
    const consumer_type get_consumer_type() const;
    const std::string& get_topic_or_queue() const;
    virtual void onException(const cms::CMSException& e) override;

protected:
    virtual void write_impl(const event& evt) override;

private:
    void init();

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

inline const activemq_writer::consumer_type activemq_writer::get_consumer_type() const
{
    return type_;
}

inline const std::string& activemq_writer::get_topic_or_queue() const
{
    return topic_or_queue_;
}

}

#endif