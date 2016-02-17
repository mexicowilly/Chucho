/*
 * Copyright 2013-2016 Will Mason
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

#include <chucho/compressing_writer.hpp>
#include <stdexcept>

namespace chucho
{

compressing_writer::compressing_writer(std::shared_ptr<formatter> fmt,
                                       std::ostream& stream,
                                       std::shared_ptr<compressor> cmp,
                                       std::shared_ptr<serializer> ser,
                                       const optional<std::size_t>& max_cache_kb,
                                       const optional<std::size_t>& max_cached_events)
    : writer(fmt),
      stream_(stream),
      max_cached_(max_cached_events),
      max_bytes_(max_cache_kb),
      serializer_(ser),
      compressor_(cmp)
{
    set_status_origin("compressing_writer");
    if (!max_bytes_ && !max_cached_)
        throw std::invalid_argument("Either max_cache_kb or max_cached_events must be set");
    if (max_bytes_)
        *max_bytes_ *= 1024;
}

compressing_writer::~compressing_writer()
{
    try
    {
        std::vector<std::uint8_t> buf;
        compressor_->finish(buf);
        if (!buf.empty())
            stream_.write(const_cast<char*>(reinterpret_cast<const char*>(&buf[0])), buf.size());
    }
    catch (...)
    {
    }
}

void compressing_writer::cache(const event& evt)
{
    auto ser = serializer_->serialize(evt, formatter_);
    std::uint32_t sz = to_network(ser.size());
    std::uint8_t* szp = reinterpret_cast<std::uint8_t*>(&sz);
    ser.insert(ser.begin(), szp, szp + sizeof(sz));
    compressor_->compress(ser, compressed_cache_);
}

void compressing_writer::flush()
{
    stream_.write(const_cast<char*>(reinterpret_cast<const char*>(&compressed_cache_[0])),
                  compressed_cache_.size());
    compressed_cache_.clear();
    events_in_cache_ = 0;
}

void compressing_writer::write_impl(const event& evt)
{
    cache(evt);
    events_in_cache_++;
    if ((max_bytes_ && compressed_cache_.size() >= *max_bytes_) ||
        (max_cached_ && events_in_cache_ >= *max_cached_))
    {
        flush();
    }
}

}