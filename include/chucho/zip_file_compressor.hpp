/*
 * Copyright 2013-2021 Will Mason
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

#if !defined(ZIP_FILE_COMPRESSOR_HPP_)
#define ZIP_FILE_COMPRESSOR_HPP_

#include <chucho/file_compressor.hpp>

namespace chucho
{

/**
 * @class zip_file_compressor zip_file_compressor.hpp chucho/zip_file_compressor.hpp 
 * Compress using the zip archive format and a file name extension of 
 * @c .zip. 
 * 
 * @ingroup fcompressors
 */
class CHUCHO_EXPORT zip_file_compressor : public file_compressor
{
public:
    /**
     * @name Constructor
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
    zip_file_compressor(unsigned min_idx);
    //@}

    virtual void compress(const std::string& file_name) override;
};

}

#endif
