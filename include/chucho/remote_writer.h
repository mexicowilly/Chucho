/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_REMOTE_WRITER_H__)
#define CHUCHO_REMOTE_WRITER_H__

/**
 * @file 
 * @copydoc chucho::remote_writer 
 *  
 * @ingroup c_writers 
 */

#include <chucho/writer.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * @copydoc chucho::remote_writer::DEFAULT_PORT
 */
extern unsigned CHUCHO_DEFAULT_REMOTE_WRITER_PORT;
/**
 * @copydoc chucho::remote_writer::DEFAULT_UNSENT_CACHE_MAX
 */
extern size_t CHUCHO_DEFAULT_REMOTE_UNSENT_CACHE_MAX;

/**
 * @name Creation
 * @{
 */
/**
 * Create a remote writer.
 * 
 * @post Ownership of the wrt parameter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_writer() function.
 * 
 * @param[out] wrt the remote writer to create
 * @param[in] host the host on which the chuchod server is 
 *       running
 * @param[in] port the port on which to connect
 * @param[in] unsent_cache_max the maximum size of the unsent 
 *                         cache size
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_remote_writer(chucho_writer** wrt,
                                                    const char* const host,
                                                    unsigned port,
                                                    size_t unsent_cache_max);
/** @}  */
/**
 * Return the host on which the chuchod server is running.
 * 
 * @param[in] wrt the remote writer
 * @param[out] host the host name
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rwrt_get_host(const chucho_writer* wrt, const char** host);
/**
 * Return the port on which the chuchod server is listening.
 * 
 * @param[in] wrt the remote writer
 * @param[out] port the port
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rwrt_get_port(const chucho_writer* wrt, unsigned* port);
/**
 * Return the maximum size of the unsent event cache.
 * 
 * @param[in] wrt the remote writer
 * @param[out] cmax the maximum size of the unsent cache
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rwrt_get_unsent_cache_max(const chucho_writer* wrt, size_t* cmax);
/**
 * Return the current size of the unsent event cache.
 * 
 * @param[in] wrt the remote writer
 * @param[out] csize the current size of the unsent cache
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_rwrt_get_unsent_cache_size(const chucho_writer* wrt, size_t* csize);

#if defined(__cplusplus)
}
#endif

#endif
