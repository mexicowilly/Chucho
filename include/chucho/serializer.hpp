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

#if !defined(CHUCHO_SERIALIZER_HPP__)
#define CHUCHO_SERIALIZER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>
#include <chucho/formatter.hpp>
#include <chucho/non_copyable.hpp>
#include <vector>
#include <cstdint>

namespace chucho
{

/**
 * @class serializer serializer.hpp chucho/serializer.hpp
 * Message queues treat messages as blobs. Serializers
 * are used by message queue writers to turn events into blobs
 * before they are sent.
 * 
 * @ingroup mq
 */
class CHUCHO_EXPORT serializer : public status_reporter,
                                 public configurable,
                                 public non_copyable
{
public:
    /**
     * Finish the blob and return it.
     *
     * @return the blob
     */
    virtual std::vector<std::uint8_t> finish_blob() = 0;
    /**
     * Turn an event into part of a blob.
     * 
     * @param evt the event to blobify
     * @param fmt the formatter used to create the formatted message
     */
    virtual void serialize(const event& evt, formatter& fmt) = 0;
};

}

#endif
