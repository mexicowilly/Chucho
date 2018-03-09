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

#include <chucho/cloudwatch_writer.hpp>
#include <chucho/exception.hpp>
#include <aws/logs/model/PutLogEventsRequest.h>
#include <aws/logs/model/DescribeLogStreamsRequest.h>
#include <aws/core/utils/Outcome.h>

namespace
{

constexpr std::size_t MAX_WIRE_SIZE = 1048576;

}

namespace chucho
{

constexpr std::size_t cloudwatch_writer::DEFAULT_BATCH_SIZE;

cloudwatch_writer::cloudwatch_writer(const std::string& name,
                                     std::unique_ptr<formatter>&& fmt,
                                     const std::string& log_group,
                                     const std::string& log_stream,
                                     std::size_t batch_size)
    : writer(name, std::move(fmt)),
      log_group_(log_group),
      log_stream_(log_stream),
      wire_size_(0),
      batch_size_(std::min(batch_size, 10000UL))
{
    set_status_origin("cloudwatch_writer");
    Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest req;
    req.SetLogGroupName(log_group_.c_str());
    req.SetLogStreamNamePrefix(log_stream_.c_str());
    req.SetOrderBy(Aws::CloudWatchLogs::Model::OrderBy::LogStreamName);
    req.SetLimit(1);
    auto oc = client_.DescribeLogStreams(req);
    if (oc.IsSuccess())
    {
        if (!oc.GetResult().GetLogStreams().empty())
            next_token_ = oc.GetResult().GetLogStreams()[0].GetUploadSequenceToken();
    }
    else
    {
        const auto& err = oc.GetError();
        if (err.GetExceptionName() != "ResourceNotFoundException")
            throw exception("AWS error describing log streams: (" + err.GetExceptionName() + ") "   + err.GetMessage());
    }
}

void cloudwatch_writer::flush()
{
    if (!events_.empty())
    {
        Aws::CloudWatchLogs::Model::PutLogEventsRequest req;
        req.SetLogGroupName(log_group_.c_str());
        req.SetLogStreamName(log_stream_.c_str());
        req.SetSequenceToken(next_token_);
        req.SetLogEvents(std::move(events_));
        wire_size_ = 0;
        auto oc = client_.PutLogEvents(req);
        if (oc.IsSuccess())
        {
            next_token_ = oc.GetResult().GetNextSequenceToken();
        }
        else
        {
            const auto &err = oc.GetError();
            throw exception("AWS error putting events: (" + err.GetExceptionName() + ") " + err.GetMessage());
        }
    }
}

void cloudwatch_writer::write_impl(const event& evt)
{
    Aws::CloudWatchLogs::Model::InputLogEvent ile;
    ile.SetTimestamp(millis_since_epoch(evt));
    ile.SetMessage(formatter_->format(evt));
    auto esz = get_wire_size(ile);
    if (events_.size() == batch_size_ || wire_size_ + esz > MAX_WIRE_SIZE)
        flush();
    events_.push_back(ile);
    wire_size_ += esz;
}

}
