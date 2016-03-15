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

#include <chucho/activemq_writer.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <cms/ConnectionFactory.h>
#include <activemq/library/ActiveMQCPP.h>
#include <thread>

namespace
{

void init_library()
{
    static std::once_flag once; 
    activemq::library::ActiveMQCPP::initializeLibrary();
}

}

namespace chucho
{

activemq_writer::activemq_writer(std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 const std::string& broker,
                                 consumer_type tp,
                                 const std::string& topic_or_queue)
    : message_queue_writer(fmt, ser),
      broker_(broker),
      type_(tp),
      topic_or_queue_(topic_or_queue),
      connection_(nullptr),
      session_(nullptr),
      destination_(nullptr),
      producer_(nullptr)
{
    init();
}

activemq_writer::activemq_writer(std::shared_ptr<formatter> fmt,
                                 std::shared_ptr<serializer> ser,
                                 std::shared_ptr<compressor> cmp,
                                 const std::string& broker,
                                 consumer_type tp,
                                 const std::string& topic_or_queue)
    : message_queue_writer(fmt, ser, cmp),
      broker_(broker),
      type_(tp),
      topic_or_queue_(topic_or_queue),
      connection_(nullptr),
      session_(nullptr),
      destination_(nullptr),
      producer_(nullptr)
{
    init();
}

activemq_writer::~activemq_writer()
{
    try
    {
        if (connection_ != nullptr)
            connection_->close();
        delete destination_;
        delete producer_;
        delete session_;
        delete connection_;
    }
    catch (std::exception& e)
    {
        report_error(std::string("Error closing the connection: ") + e.what());
    }
}

void activemq_writer::init()
{
    set_status_origin("activemq_writer");
    static std::once_flag once;
    std::call_once(once, [] () { activemq::library::ActiveMQCPP::initializeLibrary();
                                 garbage_cleaner::get().add([] () { activemq::library::ActiveMQCPP::shutdownLibrary(); }); });
    std::unique_ptr<cms::ConnectionFactory> fact(cms::ConnectionFactory::createCMSConnectionFactory(broker_));
    connection_ = fact->createConnection();
    connection_->setExceptionListener(this);
    connection_->start();
    session_ = connection_->createSession(cms::Session::AUTO_ACKNOWLEDGE);
    if (type_ == consumer_type::TOPIC)
        destination_ = session_->createTopic(topic_or_queue_);
    else
        destination_ = session_->createQueue(topic_or_queue_);
    producer_ = session_->createProducer(destination_);
}

void activemq_writer::onException(const cms::CMSException& e)
{
    report_error("ActiveMQ error: " + e.getMessage());
}

void activemq_writer::write_impl(const event& evt)
{
    auto bytes = serialize_and_compress(evt);
    std::unique_ptr<cms::BytesMessage> msg(session_->createBytesMessage());
    msg->setBodyBytes(&bytes[0], bytes.size());
    producer_->send(msg.get());
}

}