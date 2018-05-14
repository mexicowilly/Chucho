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

#include <chucho/cloudwatch_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>
#include <chucho/regex.hpp>
#include <chucho/environment.hpp>
#include <aws/logs/model/PutLogEventsRequest.h>
#include <aws/logs/model/DescribeLogStreamsRequest.h>
#include <aws/core/utils/Outcome.h>
#include <fstream>

namespace
{

constexpr const std::size_t MAX_WIRE_SIZE = 1048576;

std::string find_aws_region()
{
    std::string result;
    auto env = chucho::environment::get("AWS_REGION");
    if (env)
    {
        result = *env;
    }
    else
    {
        auto conf = chucho::file::home_directory() + ".aws" + chucho::file::dir_sep + "config";
        if (chucho::file::exists(conf))
        {
            std::ifstream in(conf);
            if (in.is_open())
            {
                chucho::regex::expression re("^[ \t]*([^ \t]+)[ \t]*=[ \t]*(.*)$");
                chucho::regex::match mch;
                std::string line;
                while (std::getline(in, line))
                {
                    if (!line.empty() && line.back() == '\r')
                        line.pop_back();
                    if (chucho::regex::search(line, re, mch) &&
                        mch.size() == 3 &&
                        mch[1].begin() != -1 &&
                        mch[2].begin() != -1 &&
                        line.substr(mch[1].begin(), mch[1].length()) == "region")
                    {
                        result = line.substr(mch[2].begin(), mch[2].length());
                        break;
                    }
                }
            }
        }
    }
    return result;
}

}

namespace chucho
{

constexpr std::size_t cloudwatch_writer::DEFAULT_BATCH_SIZE;

cloudwatch_writer::cloudwatch_writer(const std::string& name,
                                     std::unique_ptr<formatter>&& fmt,
                                     const std::string& log_group,
                                     const std::string& log_stream,
                                     std::size_t batch_size)
    : cloudwatch_writer(name, std::move(fmt), log_group, log_stream, find_aws_region(), batch_size)
{
}

cloudwatch_writer::cloudwatch_writer(const std::string& name,
                                     std::unique_ptr<formatter>&& fmt,
                                     const std::string& log_group,
                                     const std::string& log_stream,
                                     const std::string& region,
                                     std::size_t batch_size)
    : cloud_writer(name, std::move(fmt)),
      log_group_(log_group),
      log_stream_(log_stream),
      wire_size_(0),
      batch_size_(std::max(1UL, std::min(batch_size, 10000UL))),
      region_(region)
{
    set_status_origin("cloudwatch_writer");
    Aws::Client::ClientConfiguration conf;
    conf.region = region;
    client_ = std::make_unique<Aws::CloudWatchLogs::CloudWatchLogsClient>(conf);
    if (batch_size > 10000)
        report_warning("The batch size of " + std::to_string(batch_size) + " has been lowered to the maximum of 10000");
    if (batch_size == 0)
        report_warning("The batch size of 0 has been changed to 1");
    Aws::CloudWatchLogs::Model::DescribeLogStreamsRequest req;
    req.SetLogGroupName(log_group_.c_str());
    req.SetLogStreamNamePrefix(log_stream_.c_str());
    req.SetOrderBy(Aws::CloudWatchLogs::Model::OrderBy::LogStreamName);
    req.SetLimit(50);
    bool finished = false;
    while (!finished)
    {
        auto oc = client_->DescribeLogStreams(req);
        if (oc.IsSuccess())
        {
            if (oc.GetResult().GetLogStreams().empty())
            {
                break;
            }
            else
            {
                for (const auto& s : oc.GetResult().GetLogStreams())
                {
                    if (s.GetLogStreamName() == log_stream_)
                    {
                        next_token_ = s.GetUploadSequenceToken();
                        finished = true;
                        break;
                    }
                }
            }
        }
        else
        {
            const auto &err = oc.GetError();
            if (err.GetExceptionName() != "ResourceNotFoundException")
                throw exception("AWS error describing log streams: (" + err.GetExceptionName() + ") " + err.GetMessage());
            break;
        }
        if (oc.GetResult().GetNextToken().empty())
            break;
        req.SetNextToken(oc.GetResult().GetNextToken());
    }
}

void cloudwatch_writer::flush()
{
    if (!events_.empty())
    {
        Aws::CloudWatchLogs::Model::PutLogEventsRequest req;
        req.SetLogGroupName(log_group_.c_str());
        req.SetLogStreamName(log_stream_.c_str());
        if (!next_token_.empty())
            req.SetSequenceToken(next_token_);
        guard_.lock();
        req.SetLogEvents(std::move(events_));
        guard_.unlock();
        wire_size_ = 0;
        auto oc = client_->PutLogEvents(req);
        if (oc.IsSuccess())
        {
            next_token_ = oc.GetResult().GetNextSequenceToken();
        }
        else
        {
            const auto& err = oc.GetError();
            throw exception("AWS error putting events: (" + err.GetExceptionName() + ") " + err.GetMessage());
        }
    }
}

std::size_t cloudwatch_writer::get_current_batch_size()
{
    std::lock_guard<std::mutex> lock(guard_);
    return events_.size();
}

void cloudwatch_writer::write_impl(const event& evt)
{
    Aws::CloudWatchLogs::Model::InputLogEvent ile;
    ile.SetTimestamp(millis_since_epoch(evt));
    ile.SetMessage(formatter_->format(evt));
    auto esz = get_wire_size(ile);
    if (events_.size() == batch_size_ || wire_size_ + esz > MAX_WIRE_SIZE)
        flush();
    std::lock_guard<std::mutex> lock(guard_);
    events_.push_back(ile);
    wire_size_ += esz;
}

}
