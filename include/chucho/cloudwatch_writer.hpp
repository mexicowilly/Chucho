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

/**
 * @class cloudwatch_writer cloudwatch_writer.hpp chucho/cloudwatch_writer.hpp
 * Write to AWS Cloudwatch logs. The events are sent to Cloudwatch
 * in batches of up to 10,000.
 *
 * If you don't provide a region, then one will be supplied by either
 * the environment variable, AWS_REGION, or the <pre>~/.aws/config</pre>
 * file. If the region cannot be gleaned from either of those sources,
 * then it will be empty.
 *
 * @ingroup writers
 */
class CHUCHO_EXPORT cloudwatch_writer : public writer
{
public:
    /**
     * The default batch size.
     */
    static constexpr std::size_t DEFAULT_BATCH_SIZE = 50;

    /**
     * @name Constructors
     * @{
     */
    /**
     * Construct a writer.
     *
     * @param name the name
     * @param fmt the formatter
     * @param log_group the name of the log group
     * @param log_stream  the name of the log stream
     * @param batch_size the batch size
     */
    cloudwatch_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& log_group,
                      const std::string& log_stream,
                      std::size_t batch_size = DEFAULT_BATCH_SIZE);
    /**
     * Construct a writer.
     *
     * @param name the name
     * @param fmt the formatter
     * @param log_group the name of the log group
     * @param log_stream  the name of the log stream
     * @param region the region
     * @param batch_size the batch size
     */
    cloudwatch_writer(const std::string& name,
                      std::unique_ptr<formatter>&& fmt,
                      const std::string& log_group,
                      const std::string& log_stream,
                      const std::string& region,
                      std::size_t batch_size = DEFAULT_BATCH_SIZE);
    /**
     * @}
     */

    virtual void flush() override;
    /**
     * Return the batch size of this writer.
     *
     * @return the batch size
     */
    std::size_t get_batch_size() const;
    /**
     * Return the number of unsent events in the current batch.
     *
     * @return the unsent events
     */
    std::size_t get_current_batch_size();
    /**
     * Return the name of the log group.
     *
     * @return the name of the group
     */
    const std::string& get_log_group() const;
    /**
     * Return the name of the log stream.
     *
     * @return the name of the stream
     */
    const std::string& get_log_stream() const;
    /**
     * Return the name of the region. If no region was passed
     * to the constructor, then this is the region that was
     * determined by evaluating the environment variable,
     * AWS_REGION, and the file <pre>~/.aws/config</pre>.
     *
     * @return the region
     */
    const std::string& get_region() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT std::size_t get_wire_size(const Aws::CloudWatchLogs::Model::InputLogEvent& e) const;
    CHUCHO_NO_EXPORT long long millis_since_epoch(const event& e) const;

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
