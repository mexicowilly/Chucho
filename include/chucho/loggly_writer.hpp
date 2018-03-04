/*
 * Copyright 2013-2017 Will Mason
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

#ifndef CHUCHO_LOGGLY_WRITER_HPP__
#define CHUCHO_LOGGLY_WRITER_HPP__

#include <chucho/writer.hpp>

namespace chucho
{

class curl;

class CHUCHO_EXPORT loggly_writer : public writer
{
public:
    loggly_writer(const std::string& name,
                  std::unique_ptr<formatter>&& fmt,
                  const std::string& token);
    virtual ~loggly_writer();

    const std::string& get_token() const;
    bool get_verbose() const;
    void set_verbose(bool state);

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::unique_ptr<curl> curl_;
    std::string token_;
};

inline const std::string& loggly_writer::get_token() const
{
    return token_;
}

}

#endif
