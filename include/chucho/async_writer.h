/*
 * Copyright 2013-2014 Will Mason
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

#if !defined(CHUCHO_ASYNC_WRITER_H__)
#define CHUCHO_ASYNC_WRITER_H__

/**
 * @file
 * @copydoc chucho::async_writer
 *
 * @ingroup c_writers
 */

#include <chucho/writer.h>
#include <chucho/level.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @copydoc chucho::async_writer::DEFAULT_QUEUE_CAPACITY
 */
extern size_t CHUCHO_DEFAULT_ASYNC_QUEUE_CAPACITY;

/**
 * @name Creation 
 * @{ 
 */
/**
 * Create an asynchronous writer.
 *  
 * @post Ownership of the wrt parameter is transferred to the 
 *       callee and must be released with the @ref
 *       chucho_release_writer() function.
 * 
 * @param[out] async the writer to create
 * @param[in] wrt the underlying writer
 * @param[in] capacity the capacity of the blocking queue
 * @param[in] discard_threshold the level at which to discard 
 *                          events once the queue is at 80%
 *                          capacity or more
 * @param[in] flush_on_destruct whether to flush the pending
 *                              events when the writer is
 *                              destroyed
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_async_writer(chucho_writer** async,
                                                   chucho_writer* wrt,
                                                   size_t capacity,
                                                   const chucho_level* discard_threshold,
                                                   int flush_on_destruct);
/** @} */

/**
 * Return the level at which events are discarded. Once the 
 * queue has become 80% full, then events can be discarded. Any 
 * event whose level is at or below the discard threshold will 
 * be thrown away. This can be disabled with the level returned 
 * by @ref chucho_off_level(). 
 * 
 * @param[in] wrt the asynchronous writer 
 * @param[out] lvl the level 
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_aswrt_get_discard_threshold(const chucho_writer* wrt, const chucho_level** lvl);
/**
 * Return whether this writer should flush any cached events at 
 * destruction time. 
 * 
 * @param[in] wrt the asynchronous writer 
 * @param[out] flsh whether the writer flushes cached events at 
 *       destruction time
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_aswrt_get_flush_on_destruct(const chucho_writer* wrt, int* flsh);
/**
 * Return the capacity of the blocking queue.
 * 
 * @param[in] wrt the asynchronous writer
 * @param[out] cap the capacity of the queue
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_aswrt_get_queue_capacity(const chucho_writer* wrt, size_t* cap);
/**
 * Return the current size of the blocking queue.
 * 
 * @param[in] wrt the asynchronous writer
 * @param[out] sz the size of the queue
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_aswrt_get_queue_size(const chucho_writer* wrt, size_t* sz);
/**
 * Return the underlying slow writer.
 *  
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller and must be released with the @ref
 *       chucho_release_writer() function.
 * 
 * @param[in] async the asynchronous writer
 * @param[out] wrt the underlying writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_aswrt_get_writer(const chucho_writer* async, chucho_writer** wrt);

#if defined(__cplusplus)
}
#endif

#endif
