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

#if !defined(LZMA_FILE_COMPRESSOR_HPP__)
#define LZMA_FILE_COMPRESSOR_HPP__

#include <chucho/file_compressor.hpp>
#include <chucho/non_copyable.hpp>
#include <lzma.h>

namespace chucho
{

/**
 * @class lzma_file_compressor lzma_file_compressor.hpp chucho/lzma_file_compressor.hpp 
 * Compress using the bzip format and a file name extension of 
 * @c .xz.
 * 
 * @ingroup fcompressors
 */
class CHUCHO_EXPORT lzma_file_compressor : public file_compressor, non_copyable
{
public:
    /**
     * @name Constructor and Destructor
     */
    //@{
    /**
     * Construct a compressor.
     * 
     * @param min_idx the minimum index at which to start 
     *                compressing
     *  
     * @sa get_min_index 
     */
    lzma_file_compressor(unsigned min_idx);
    ~lzma_file_compressor();
    //@}

    virtual void compress(const std::string& file_name) override;

private:
    lzma_stream lzma_;
};

}

#endif
