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

#if !defined(CHUCHO_ASYNC_WRITER_HPP__)
#define CHUCHO_ASYNC_WRITER_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/writer.hpp>
#include <queue>
#include <thread>
#include <condition_variable>

namespace chucho
{

/**
 * @class async_writer async_writer.hpp chucho/async_writer.hpp 
 * An asynchronous writer. An asynchronous writer attaches to 
 * another writer that might be going too slowly and forwards 
 * events to it in a second thread. This can allow applications 
 * not to be slowed by slow writers. 
 *  
 * An asynchronous writer holds its events in a queue, the 
 * capacity of which can be set. This is a blocking queue, 
 * meaning that once the queue is full, then the asynchronous 
 * writer pushes back on the application and becomes synchronous 
 * again. 
 *  
 * The push-back is mitigated by the fact that the asynchronous 
 * writer can drop events of an indicated level. Once the queue 
 * is 80% full, then events at the discard threshold are 
 * dropped. By default the discard threshold is level INFO_. 
 * This means that any event with INFO_ level or less gets 
 * dropped. With the default set of levels, only WARN_, ERROR_ 
 * and FATAL_ events will get written to the underlying slow 
 * writer. 
 * 
 * @ingroup writers
 */
class CHUCHO_EXPORT async_writer : public writer
{
public:
    /**
     * The default queue capacity, which is 256.
     */
    static const std::size_t DEFAULT_QUEUE_CAPACITY;

    /**
     * @name Constructor and destructor
     */
    //@{
    /**
     * Construct an asynchronous writer.
     *
     * @param name the name of the writer
     * @param wrt the underlying slow writer
     * @param capacity the capacity of the blocking queue
     * @param discard_threshold the level at which to discard events 
     *                          once the queue is at 80% capacity or
     *                          more
     * @param flush_on_destruct whether to flush the pending events 
     *                          when the writer is destroyed
     * @throw std::invalid_argument if fmt is an uninitialized 
     *        std::shared_ptr
     */
    async_writer(const std::string& name,
                 std::shared_ptr<writer> wrt,
                 std::size_t capacity = DEFAULT_QUEUE_CAPACITY,
                 std::shared_ptr<level> discard_threshold = level::INFO_(),
                 bool flush_on_destruct = true);
    /**
     * Destruct an asynchronous writer.
     */
    ~async_writer();
    //@}

    /**
     * Return the level at which events are discarded. Once the 
     * queue has become 80% full, then events can be discarded. Any 
     * event whose level is at or below the discard threshold will 
     * be thrown away. This can be disabled with level OFF_. 
     * 
     * @return the discard threshold
     */
    std::shared_ptr<level> get_discard_threshold() const;
    /**
     * Return whether this writer should flush any cached events at 
     * destruction time. 
     * 
     * @return whether the writer flushes on destruct
     */
    bool get_flush_on_destruct() const;
    /**
     * Return the queue capacity.
     * 
     * @return the queue capacity
     */
    std::size_t get_queue_capacity() const;
    /**
     * Return the queue size. This is the number of events that are 
     * currently in the queue. 
     * 
     * @return the queue size
     */
    std::size_t get_queue_size();
    /**
     * Return the underlying slow writer.
     * 
     * @return the slow writer
     */
    std::shared_ptr<writer> get_writer() const;

protected:
    virtual void write_impl(const event& evt) override;

private:
    friend class mysql_writer;

    CHUCHO_NO_EXPORT async_writer(std::shared_ptr<writer> wrt,
                                  std::size_t capacity,
                                  std::shared_ptr<level> discard_threshold,
                                  bool flush_on_destruct,
                                  std::function<void()> enter_thread_cb,
                                  std::function<void()> leave_thread_cb);

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
    std::function<void()> enter_thread_cb_;
    std::function<void()> leave_thread_cb_;
    bool flush_on_destruct_;
};

inline std::shared_ptr<level> async_writer::get_discard_threshold() const
{
    return discard_threshold_;
}

inline bool async_writer::get_flush_on_destruct() const
{
    return flush_on_destruct_;
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

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
