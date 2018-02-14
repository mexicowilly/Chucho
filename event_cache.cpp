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

#include <chucho/event_cache.hpp>
#include <chucho/file.hpp>
#include <chucho/process.hpp>
#include <chucho/logger.hpp>
#include <sstream>
#include <thread>
#include <algorithm>
#include <cassert>

namespace
{

int get_random_number()
{
    static std::mutex guard;
    static std::once_flag once;

    std::lock_guard<std::mutex> lock(guard);
    std::call_once(once, [] () { std::srand(std::time(nullptr)); });
    return std::rand();
}

}

namespace chucho
{

using namespace std::chrono_literals;

event_cache::event_cache(std::size_t chunk_size, std::size_t max_size, cull_callback cull_cb)
    : chunk_size_(chunk_size),
      max_size_(max_size),
      directory_(file::temporary_directory() + "chucho-cache" + file::dir_sep + std::to_string(process::id()) +
                 file::dir_sep + std::to_string(get_random_number()) + file::dir_sep),
      mem_chunk_(std::make_unique<std::uint8_t[]>(chunk_size)),
      read_pos_(mem_chunk_.get()),
      write_pos_(mem_chunk_.get()),
      current_sequence_(0),
      should_stop_(false),
      total_size_(0),
      mem_chunk_occupied_(0),
      cull_cb_(cull_cb)
{
    if (chunk_size_ >= max_size_)
        throw std::invalid_argument("max_size must be greater than chunk_size");
    set_status_origin("event_cache");
}

event_cache::~event_cache()
{
    try
    {
        stop();
    } catch (...)
    {
    }
}

void event_cache::cull()
{
    std::size_t culled = 0;
    if (mem_chunk_occupied_ > 0)
    {
        std::memset(mem_chunk_.get(), 0, chunk_size_);
        culled = mem_chunk_occupied_;
        mem_chunk_occupied_ = 0;
    }
    else
    {
        auto oldest = find_oldest_file();
        culled = file::size(oldest);
        file::remove(oldest);
    }
    total_size_ -= culled;
    report_warning("The cache is full. Removed " + std::to_string(culled) + " oldest bytes.");
    if (cull_cb_)
        cull_cb_(culled);
}

std::string event_cache::find_oldest_file()
{
    file::directory_iterator itor(directory_);
    file::directory_iterator end;
    std::vector<std::string> files;
    while (itor != end)
    {
        files.push_back(*itor);
        ++itor;
    }
    auto mn = std::min_element(files.begin(), files.end(),
                               [] (const std::string& s1, const std::string& s2)
                               {
                                   return std::stoul(file::base_name(s1)) < std::stoul(file::base_name(s2));
                               });
    if (mn == files.end())
        throw std::runtime_error("No event files were found");
    return *mn;
}

optional<event> event_cache::pop()
{
    optional<event> result;
    std::unique_lock<std::mutex> lock(guard_);
    while (!should_stop_ && total_size_ == 0)
        read_cond_.wait_for(lock, 250ms);
    if (!should_stop_)
    {
        if (read_pos_ >= (mem_chunk_.get() + chunk_size_ - 4) || get_mem_buf<std::uint32_t>(0) == 0)
        {
            auto oldest = find_oldest_file();
            std::memset(mem_chunk_.get(), 0, chunk_size_);
            auto sz = file::size(oldest);
            std::ifstream stream(oldest, std::ios::in | std::ios::binary);
            stream.read(reinterpret_cast<char*>(mem_chunk_.get()), sz);
            stream.close();
            read_pos_ = mem_chunk_.get();
            mem_chunk_occupied_ = sz;
            assert(write_file_);
            if (write_file_ && std::stoul(file::base_name(oldest)) == current_sequence_)
            {
                write_pos_ = mem_chunk_.get() + static_cast<std::size_t>(write_file_->tellp());
                write_file_.reset();
            }
            file::remove(oldest);
            report_info("Loaded file " + oldest);
        }
        std::size_t sz;
        result = unserialize(sz);
        total_size_ -= sz;
        read_pos_ += sz;
    }
    return result;
}

void event_cache::push(const event& evt)
{
    std::lock_guard<std::mutex> lock(guard_);
    if (total_size_ >= max_size_)
        cull();
    auto sz = serialize(evt);
    if (write_pos_ != nullptr && (write_pos_ - mem_chunk_.get()) + sz <= chunk_size_)
    {
        std::memcpy(write_pos_, &ser_buf_[0], sz);
        write_pos_ += sz;
        mem_chunk_occupied_ += sz;
    }
    else
    {
        write_pos_ = nullptr;
        if (!write_file_ || static_cast<std::size_t>(write_file_->tellp()) + sz > chunk_size_)
        {
            if (!file::exists(directory_))
                file::create_directories(directory_);
            std::string fn = directory_ + std::to_string(++current_sequence_);
            write_file_ = std::make_unique<std::ofstream>(fn, std::ios::out | std::ios::binary);
            if (!write_file_->is_open())
                throw std::runtime_error("Unable to open " + fn);
            report_info("Started new file " + fn);
        }
        write_file_->write(reinterpret_cast<char*>(&ser_buf_[0]), sz);
        write_file_->flush();
    }
    total_size_ += sz;
    read_cond_.notify_one();
}

std::size_t event_cache::serialize(const event& evt)
{
    std::string mrk_text;
    std::string thr_text;
    auto sz = serialized_size(evt, mrk_text, thr_text);
    if (ser_buf_.size() < sz)
        ser_buf_.resize(sz);
    std::size_t pos = 0;
    set_ser_buf<std::uint32_t>(pos, sz);
    pos += 4;
    std::size_t len = evt.get_logger()->get_name().length();
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], evt.get_logger()->get_name().data(), len);
    pos += len;
    len = std::strlen(evt.get_level()->get_name());
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], evt.get_level()->get_name(), len);
    pos += len;
    len = evt.get_message().length();
    set_ser_buf<std::uint32_t>(pos, len);
    pos += 4;
    std::memcpy(&ser_buf_[pos], evt.get_message().data(), len);
    pos += len;
    set_ser_buf<std::uint64_t>(pos, std::chrono::duration_cast<std::chrono::microseconds>(evt.get_time().time_since_epoch()).count());
    pos += 8;
    len = std::strlen(evt.get_file_name());
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], evt.get_file_name(), len);
    pos += len;
    set_ser_buf<std::uint32_t>(pos, evt.get_line_number());
    pos += 4;
    len = std::strlen(evt.get_function_name());
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], evt.get_function_name(), len);
    pos += len;
    len = mrk_text.length();
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], mrk_text.data(), len);
    pos += len;
    len = thr_text.length();
    set_ser_buf<std::uint16_t>(pos, len);
    pos += 2;
    std::memcpy(&ser_buf_[pos], thr_text.data(), len);
    return sz;
}

std::size_t event_cache::serialized_size(const event& evt, std::string& mrk_text, std::string& thr_text)
{
    std::size_t sz = 4 +
                     2 + evt.get_logger()->get_name().length() +
                     2 + std::strlen(evt.get_level()->get_name()) +
                     4 + evt.get_message().length() +
                     8 +
                     2 + std::strlen(evt.get_file_name()) +
                     4 +
                     2 + std::strlen(evt.get_function_name()) +
                     2;
    if (evt.get_marker())
    {
        std::ostringstream stream;
        stream << *evt.get_marker();
        mrk_text = stream.str();
        sz += mrk_text.length();
    }
    sz += 2;
    std::ostringstream stream;
    stream << std::this_thread::get_id();
    thr_text = stream.str();
    sz += thr_text.length();
    return sz;
}

void event_cache::stop()
{
    std::lock_guard<std::mutex> lock(guard_);
    should_stop_ = true;
    write_file_.reset();
    file::remove_all(directory_);
    auto parent = file::directory_name(directory_);
    bool empty = false;
    {
        file::directory_iterator itor(parent);
        file::directory_iterator end;
        empty = itor == end;
    }
    if (empty)
    {
        file::remove(parent);
        auto grandparent = file::directory_name(parent);
        empty = false;
        {
            file::directory_iterator itor(grandparent);
            file::directory_iterator end;
            empty = itor == end;
        }
        if (empty)
            file::remove(grandparent);
    }
}

event event_cache::unserialize(std::size_t& sz)
{
    sz = get_mem_buf<std::uint32_t>(0);
    std::size_t pos = 4;
    std::size_t len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto lgr_name = get_mem_buf_str(pos, len);
    pos += len;
    len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto lvl_name = get_mem_buf_str(pos, len);
    pos += len;
    len = get_mem_buf<std::uint32_t>(pos);
    pos += 4;
    auto msg = get_mem_buf_str(pos, len);
    pos += len;
    auto usecs = get_mem_buf<std::uint64_t>(pos);
    pos += 8;
    len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto fn = get_mem_buf_str(pos, len);
    pos += len;
    auto line = get_mem_buf<std::uint32_t>(pos);
    pos += 4;
    len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto func = get_mem_buf_str(pos, len);
    pos += len;
    len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto mrk = get_mem_buf_str(pos, len);
    pos += len;
    len = get_mem_buf<std::uint16_t>(pos);
    pos += 2;
    auto thr = get_mem_buf_str(pos, len);
    optional<marker> omrk;
    if (!mrk.empty())
        omrk = mrk;
    event result(logger::get(lgr_name),
                 level::from_text(lvl_name),
                 msg,
                 nullptr,
                 line,
                 nullptr,
                 omrk);
    result.file_name_store_ = fn;
    result.file_name_ = result.file_name_store_->c_str();
    result.function_name_store_ = func;
    result.function_name_ = result.function_name_store_->c_str();
    result.thread_id_ = thr;
    result.time_ = event::clock_type::time_point();
    result.time_ += std::chrono::microseconds(usecs);
    return result;
}

}
