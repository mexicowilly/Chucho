/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_MESSAGE_QUEUE_WRITER_HPP__)
#define CHUCHO_MESSAGE_QUEUE_WRITER_HPP__

#include <chucho/writer.hpp>
#include <chucho/serializer.hpp>

namespace chucho
{

class CHUCHO_EXPORT message_queue_writer : public writer
{
protected:
    /**
     * @name Constructor
     */
    message_queue_writer(std::shared_ptr<formatter> fmt,
                         std::shared_ptr<serializer> ser);
    //@}

    std::shared_ptr<serializer> serializer_;
};

}

#endif