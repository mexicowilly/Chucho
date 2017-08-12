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

#if !defined(CHUCHO_MESSAGE_QUEUE_WRITER_H__)
#define CHUCHO_MESSAGE_QUEUE_WRITER_H__

/**
 * @file
 * Functions that can be used with any of the message queue writers created by
 * one of the creation functions.
 *
 * @ingroup c_mq
 */

#include <chucho/writer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Return the maximum number of events that the writer will collect before
 * sending them to the message queue.
 *
 * @param[in] wrt the message queue writer
 * @param[out] cmax the maximum
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_mqwrt_get_coalesce_max(const chucho_writer* wrt, size_t* cmax);

#if defined(__cplusplus)
}
#endif

#endif
