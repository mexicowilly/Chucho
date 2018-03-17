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
#include <aws/logs/model/InputLogEvent.h>

namespace chucho
{

class CHUCHO_EXPORT cloudwatch_writer : public writer
{
public:
    static constexpr std::size_t DEFAULT_BATCH_SIZE = 50;

    cloudwatch_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& log_group,
                      const std::string& log_stream,
                      std::size_t batch_size = DEFAULT_BATCH_SIZE);
    cloudwatch_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& log_group,
                      const std::string& log_stream,
                      const std::string& region,
                      std::size_t batch_size = DEFAULT_BATCH_SIZE);

    virtual void flush() override;
    std::size_t get_batch_size() const;
    std::size_t get_current_batch_size();
    const std::string& get_log_group() const;
    const std::string& get_log_stream() const;
    const std::string& get_region() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    std::size_t get_wire_size(const Aws::CloudWatchLogs::Model::InputLogEvent& e) const;
    long long millis_since_epoch(const event& e) const;

    std::unique_ptr<Aws::CloudWatchLogs::CloudWatchLogsClient> client_;
    std::string log_group_;
    std::string log_stream_;
    Aws::Vector<Aws::CloudWatchLogs::Model::InputLogEvent> events_;
    std::size_t wire_size_;
    Aws::String next_token_;
    std::size_t batch_size_;
    std::string region_;
    std::mutex guard_;
};

inline std::size_t cloudwatch_writer::get_batch_size() const
{
    return batch_size_;
}

inline const std::string& cloudwatch_writer::get_log_group() const
{
    return log_group_;
}

inline const std::string& cloudwatch_writer::get_log_stream() const
{
    return log_stream_;
}

inline const std::string& cloudwatch_writer::get_region() const
{
    return region_;
}

inline std::size_t cloudwatch_writer::get_wire_size(const Aws::CloudWatchLogs::Model::InputLogEvent& e) const
{
    return e.GetMessage().length() + 26;
}

inline long long cloudwatch_writer::millis_since_epoch(const event& e) const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(e.get_time().time_since_epoch()).count();
}

}

#endif
