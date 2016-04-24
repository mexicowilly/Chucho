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

#if !defined(CHUCHO_ZLIB_COMPRESSOR_H__)
#define CHUCHO_ZLIB_COMPRESSOR_H__

/**
 * @file 
 * @copydoc chucho::zlib_compressor 
 *  
 * @ingroup c_compressors 
 */

#include <chucho/compressor.h>
#include <zlib.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a zlib compressor.
 * 
 * @post Ownership of the cmp paramter is transferred to the 
 *       caller, and it must be released with the @ref
 *       chucho_release_compressor() function.
 * 
 * @param[out] cmp the compressor to create
 * @param[in] compression_level the level of compression in the range
 * [0, 9]
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_zlib_compressor(chucho_compressor** ser,
                                                      int compression_level = Z_DEFAULT_COMPRESSION);

#if defined(__cplusplus)
}
#endif

#endif