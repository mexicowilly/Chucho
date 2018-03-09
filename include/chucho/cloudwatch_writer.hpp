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

#ifndef CHUCHO_CLOUDWATCH_WRITER_HPP__
#define CHUCHO_CLOUDWATCH_WRITER_HPP__

#include <chucho/writer.hpp>
#include <aws/logs/CloudWatchLogsClient.h>

namespace chucho
{

class CHUCHO_EXPORT cloudwatch_writer : public writer
{
public:
    cloudwatch_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& log_group,
                      const std::string& log_stream);
    virtual ~cloudwatch_writer();

    virtual void flush() override;
    const std::string& get_log_group() const;
    const std::string& get_log_stream() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    struct aws_event
    {
        aws_event(formatter& fmt, const event& evt);

        std::size_t get_wire_size() const;

        long long millis_since_epoch;
        Aws::String message;
    };

    Aws::CloudWatchLogs::CloudWatchLogsClient client_;
    std::string log_group_;
    std::string log_stream_;
    std::vector<aws_event> aws_events_;
    std::size_t wire_size_;
    Aws::String next_token_;
};

inline const std::string& cloudwatch_writer::get_log_group() const
{
    return log_group_;
}

inline const std::string& cloudwatch_writer::get_log_stream() const
{
    return log_stream_;
}

}

#endif
