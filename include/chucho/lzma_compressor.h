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

#if !defined(CHUCHO_LZMA_COMPRESSOR_H__)
#define CHUCHO_LZMA_COMPRESSOR_H__

/**
 * @file
 * @copydoc chucho::lzma_compressor
 *
 * @ingroup c_compressors
 */

#include <chucho/compressor.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Create a lzma compressor.
 *
 * @post Ownership of the cmp paramter is transferred to the
 *       caller, and it must be released with the @ref
 *       chucho_release_compressor() function.
 *
 * @param[out] cmp the compressor to create
 * @return a value from @ref return_code.h indicating success or
 *         failure
 */
CHUCHO_EXPORT chucho_rc chucho_create_lzma_compressor(chucho_compressor** cmp);

#if defined(__cplusplus)
}
#endif

#endif
