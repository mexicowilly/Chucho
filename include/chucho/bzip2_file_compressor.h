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

#if !defined(CHUCHO_BZIP2_FILE_COMPRESSOR_H__)
#define CHUCHO_BZIP2_FILE_COMPRESSOR_H__

/**
 * @file 
 * Creation of bzip2 file compressors. 
 * @copydoc chucho::bzip2_file_compressor
 *  
 * @ingroup c_compressors 
 */

#include <chucho/file_compressor.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a bzip2 file compressor.
 *  
 * @post Ownership of the cmp parameters is transferred to the 
 *       caller, and must be released with the @ref
 *       chucho_release_file_compressor function.
 * 
 * @param[out] cmp the compressor to create
 * @param[in] min_index the minimum index at which to start 
 *                      compressing during a roll operation
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_bzip2_file_compressor(chucho_file_compressor** cmp, unsigned min_index);

#if defined(__cplusplus)
}
#endif

#endif
