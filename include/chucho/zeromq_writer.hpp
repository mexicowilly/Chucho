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

#if !defined(CHUCHO_ZEROMQ_WRITER_HPP__)
#define CHUCHO_ZEROMQ_WRITER_HPP__

#include <chucho/message_queue_writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT zeromq_writer : public message_queue_writer
{
public:
    zeromq_writer(std::shared_ptr<formatter> fmt,
                  std::shared_ptr<serializer> ser,
                  const std::string& endpoint,
                  const std::vector<std::uint8_t>& prefix = std::vector<std::uint8_t>());
    ~zeromq_writer();

    const std::string& get_endpoint() const;
    const std::vector<std::uint8_t>& get_prefix() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::string endpoint_;
    std::vector<std::uint8_t> prefix_;
    void* socket_;
};

inline const std::string& zeromq_writer::get_endpoint() const
{
    return endpoint_;
}

inline const std::vector<std::uint8_t>& zeromq_writer::get_prefix() const
{
    return prefix_;
}

}

#endif