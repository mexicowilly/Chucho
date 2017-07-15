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

#if !defined(CHUCHO_FILE_COMPRESSOR_H__)
#define CHUCHO_FILE_COMPRESSOR_H__

/**
 * @file 
 * Functions common to all file compressors. 
 * @copydoc chucho::file_compressor
 *  
 * @ingroup c_fcompressors 
 */

#include <chucho/return_code.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * An opaque type that you don't care about.
 */
typedef struct chucho_file_compressor chucho_file_compressor;

/**
 * Release a file compressor. 
 *  
 * @post Ownership of the cmp paramter is transferred to the 
 *       callee.
 * 
 * @param[in] cmp the file compressor
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_release_file_compressor(chucho_file_compressor* cmp);
/**
 * Return the file name extension used by the given file 
 * compressor. 
 * 
 * @param[in] cmp the file compressor
 * @param[out] ext the file name extension
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_cmp_get_extension(const chucho_file_compressor* cmp, const char** ext);
/**
 * Return the minimum index of rolled files at which to start 
 * compressing them. 
 *
 * @param[in] cmp the file compressor
 * @param[out] idx the minimum index
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_cmp_get_min_index(const chucho_file_compressor* cmp, unsigned* idx);

#if defined(__cplusplus)
}
#endif

#endif
