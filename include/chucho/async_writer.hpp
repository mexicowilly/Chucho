/*
 * Copyright 2013 Will Mason
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

#if !defined(CHUCHO_ASYNC_WRITER_HPP__)
#define CHUCHO_ASYNC_WRITER_HPP__

#include <chucho/writer.hpp>
#include <queue>
#include <thread>

namespace chucho
{

class CHUCHO_EXPORT async_writer : public writer
{
public:
    static const std::size_t DEFAULT_QUEUE_CAPACITY;

    async_writer(std::shared_ptr<writer> wrt,
                 std::size_t capacity = DEFAULT_QUEUE_CAPACITY,
                 std::shared_ptr<level> discard_threshold = level::INFO_());
    ~async_writer();

    std::shared_ptr<level> get_discard_threshold() const;
    std::size_t get_queue_capacity() const;
    std::size_t get_queue_size();
    std::shared_ptr<writer> get_writer() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    CHUCHO_NO_EXPORT void thread_main();

    std::deque<event> queue_;
    std::shared_ptr<writer> writer_;
    std::size_t capacity_;
    std::mutex guard_;
    std::condition_variable full_condition_;
    std::condition_variable empty_condition_;
    std::shared_ptr<level> discard_threshold_;
    bool stop_;
    std::unique_ptr<std::thread> worker_;
};

inline std::shared_ptr<level> async_writer::get_discard_threshold() const
{
    return discard_threshold_;
}

inline std::size_t async_writer::get_queue_capacity() const
{
    return capacity_;
}

inline std::shared_ptr<writer> async_writer::get_writer() const
{
    return writer_;
}

}

#endif
