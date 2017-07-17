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

#if !defined(CHUCHO_STREAMABLE_HPP__)
#define CHUCHO_STREAMABLE_HPP__

#include <chucho/loggable.hpp>
#include <chucho/log_stream.hpp>

namespace chucho
{

template <typename type>
class streamable : public loggable<type>
{
protected:
    streamable(std::shared_ptr<level> lvl = std::shared_ptr<level>());
    streamable(const std::string& name,
               std::shared_ptr<level> lvl = std::shared_ptr<level>());
    streamable(const streamable& other);
    streamable(streamable&& other);

    streamable& operator= (const streamable& other);

    log_stream& get_log_stream() const;
    virtual void rename_logger(const std::type_info& new_type) override;
    virtual void rename_logger(const std::string& name) override;

private:
    std::unique_ptr<log_stream> log_stream_;
};

#define CHUCHO_MS CHUCHO_M(get_log_stream())

template <typename type>
streamable<type>::streamable(std::shared_ptr<level> lvl)
    : log_stream_(new log_stream(loggable<type>::get_logger(), lvl))
{
}

template <typename type>
streamable<type>::streamable(const std::string& name,
                             std::shared_ptr<level> lvl)
    : loggable<type>(name),
      log_stream(new log_stream(loggable<type>::get_logger(), lvl))
{
}

template <typename type>
streamable<type>::streamable(const streamable& other)
    : log_stream_(new log_stream(loggable<type>::get_logger(), other.log_stream_->get_level()))
{
    loggable<type>::rename_logger(other.get_logger()->get_name());
}

template <typename type>
streamable<type>::streamable(streamable&& other)
    : loggable<type>(other.get_logger()->get_name()),
      log_stream_(std::move(other.log_stream_))
{
}

template <typename type>
streamable<type>& streamable<type>::operator= (const streamable& other)
{
    if (&other != this)
    {
        loggable<type>::rename_logger(other.get_logger()->get_name());
        log_stream_.reset(new log_stream(other.get_logger(), other.log_stream_->get_level()));
    }
    return *this;
}

template <typename type>
log_stream& streamable<type>::get_log_stream() const
{
    return *log_stream_;
}

template <typename type>
void streamable<type>::rename_logger(const std::type_info& new_type)
{
    loggable<type>::rename_logger(new_type);
    log_stream_.reset(new log_stream(loggable<type>::get_logger(), log_stream_->get_level()));
}

template <typename type>
void streamable<type>::rename_logger(const std::string& name)
{
    loggable<type>::rename_logger(name);
    log_stream_.reset(new log_stream(loggable<type>::get_logger(), log_stream_->get_level()));
}

}

#endif
