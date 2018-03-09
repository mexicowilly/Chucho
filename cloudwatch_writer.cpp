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
constexpr std::size_t MAX_EVENT_COUNT = 10000;

}

namespace chucho
{

cloudwatch_writer::cloudwatch_writer(const std::string& name,
                                     std::unique_ptr<formatter>&& fmt,
                                     const std::string& log_group,
                                     const std::string& log_stream)
    : writer(name, std::move(fmt)),
      log_group_(log_group),
      log_stream_(log_stream),
      wire_size_(0)
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

cloudwatch_writer::~cloudwatch_writer()
{
    try
    {
        flush();
    }
    catch (...)
    {
    }
}

void cloudwatch_writer::flush()
{
    if (!aws_events_.empty())
    {
        Aws::CloudWatchLogs::Model::PutLogEventsRequest req;
        req.SetLogGroupName(log_group_.c_str());
        req.SetLogStreamName(log_stream_.c_str());
        req.SetSequenceToken(next_token_);
        for (const auto &e : aws_events_)
        {
            req.AddLogEvents(Aws::CloudWatchLogs::Model::InputLogEvent().
                WithTimestamp(e.millis_since_epoch).WithMessage(e.message.c_str()));
        }
        auto oc = client_.PutLogEvents(req);
        if (oc.IsSuccess())
        {
            next_token_ = oc.GetResult().GetNextSequenceToken();
            aws_events_.clear();
            wire_size_ = 0;
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
    aws_event aevt(*formatter_, evt);
    auto esz = aevt.get_wire_size();
    if (aws_events_.size() == MAX_EVENT_COUNT || wire_size_ + esz > MAX_WIRE_SIZE)
        flush();
    aws_events_.push_back(aevt);
    wire_size_ += esz;
}

cloudwatch_writer::aws_event::aws_event(formatter& fmt, const event& evt)
    : millis_since_epoch(std::chrono::duration_cast<std::chrono::milliseconds>(evt.get_time().time_since_epoch()).count()),
      message(fmt.format(evt))
{
}

std::size_t cloudwatch_writer::aws_event::get_wire_size() const
{
    return message.length() + 26;
}

}
