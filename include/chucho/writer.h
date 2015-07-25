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

#if !defined(CHUCHO_WRITER_H__)
#define CHUCHO_WRITER_H__

/**
 * @file 
 * Functions that can be used with any of the writers created by 
 * one of the writer creation functions. 
 *  
 * @ingroup c_writers 
 */

#include <chucho/filter.h>
#include <chucho/formatter.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_writer chucho_writer;

/**
 * Release a writer.
 *  
 * @post Ownership of the wrt parameter is transferred to the 
 *       callee.
 * 
 * @param[in] wrt the writer to release
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_writer(chucho_writer* wrt);
/**
 * Add a filter to the writer.
 *  
 * @post Ownership of the flt parameter is transferred to the 
 *       callee.
 * 
 * @param[in] wrt the writer
 * @param[in] flt the filter
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_wrt_add_filter(chucho_writer* wrt, chucho_filter* flt);
/**
 * Remove all filters from the writer.
 * 
 * @param[in] wrt the writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_wrt_clear_filters(chucho_writer* wrt);
/**
 * Retrieve all filters. An array of filters is provided, which 
 * will be filled. If the number of existing filters in the 
 * writer exceeds the size indicated by the buf_size paramter, 
 * then CHUCHO_INSUFFICIENT_BUFFER is returned. The number of 
 * existing filters is always return in the count parameters. 
 * You may pass NULL for the buf parameters if you just want to 
 * find out how many filters there are. 
 *  
 * @post The ownership of all filters in the returned array is 
 *       transferred to the caller, and they must be releaesd
 *       with the @ref chucho_release_filter() function.
 * 
 * @param[in] wrt the writer
 * @param[out] buf the array to receive the filters
 * @param[in] buf_size the size of the array
 * @param[out] count the actual number of filters in the writer
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_wrt_get_filters(const chucho_writer* wrt, chucho_filter** buf, size_t buf_size, size_t* count);
/**
 * Retrieve the formatter for this writer.
 *  
 * @post The ownership of the fmt parameter is transferred to 
 *       the caller, and it must be released with the @ref
 *       chucho_release_formatter() function.
 * 
 * @param[in] wrt the writer
 * @param[out] fmt the formatter
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_wrt_get_formatter(const chucho_writer* wrt, chucho_formatter** fmt);

#if defined(__cplusplus)
}
#endif

#endif
