/*
 * Copyright 2013-2019 Will Mason
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

#if !defined(CHUCHO_FILE_COMPRESSOR_HPP__)
#define CHUCHO_FILE_COMPRESSOR_HPP__

#include <chucho/configurable.hpp>
#include <string>

namespace chucho
{

/**
 * @class file_compressor file_compressor.hpp chucho/file_compressor.hpp 
 * File compressors are able to compress files. They take as 
 * input the name of a file to compress, and they create a file 
 * with the same name plus an extension specific to the 
 * compressor, for example ".gz" in the case of gzip 
 * compression. A compressor is optionally attached to a @ref 
 * file_roller. 
 *  
 * Each compressor has a minimum index at which it should start 
 * to compress files. Since each @ref file_roller can have its 
 * own strategy for rolling, the compressor index is always 
 * counted sequentially from the most recent rolled file, which 
 * has an index of 1. So, if you set the minimum index to 2, 
 * then the first roll operation will leave its file 
 * uncompressed, and all subsequent ones will be compressed. 
 *  
 * For example, if you have a @ref numbered_file_roller and a 
 * @ref gzip_file_compressor with a minimum index of 2, then you 
 * will see a sequence like the following: 
 * @code 
 * my.log 
 * my.log.1 
 * my.log.2.gz 
 * my.log.3.gz 
 * @endcode 
 * 
 * @ingroup fcompressors
 */
class CHUCHO_EXPORT file_compressor : public configurable
{
public:
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destruct the compressor.
     */
    virtual ~file_compressor() { }
    //@}

    /**
     * Compress the named file, creating a new, compressed file with 
     * the file's same name and an extension corresponding to this 
     * compressor. 
     * 
     * @param file_name the name of the input file 
     *  
     * @sa get_extension 
     */
    virtual void compress(const std::string& file_name) = 0;
    /**
     * Return the file name extension used by this compressor.
     * 
     * @return const char* the extension
     */
    const char* get_extension() const;
    /**
     * Return the minimum index at which to start compressing rolled 
     * files. The index here may have no relation to an index that 
     * may be associated with the @ref file_roller, since each 
     * roller can roll files however it wants. 
     *  
     * The indexes in terms of the compressor count from most 
     * recent, with index 1, to leaat recent. 
     * 
     * For example, if you have a @ref numbered_file_roller and a 
     * @ref gzip_file_compressor with a minimum index of 2, then you 
     * will see a sequence like the following: 
     * @code 
     * my.log 
     * my.log.1 
     * my.log.2.gz 
     * my.log.3.gz 
     * @endcode 
     *
     * @return unsigned the minimum index
     */
    unsigned get_min_index() const;

protected:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct the compressor.
     */
    /**
     * Create a compressor with the given minimum index and file 
     * name extension. 
     * 
     * @param min_idx the minimum index at which to start 
     *                compressing
     * @param extension the file name extension for this compressor
     *  
     * @sa get_min_index, get_extension 
     */
    file_compressor(unsigned min_idx, const char* extension);
    //@}

private:
    const char* extension_;
    unsigned min_index_;
};

inline file_compressor::file_compressor(unsigned min_idx, const  char* extension)
    : extension_(extension),
      min_index_(min_idx)
{
}

inline const char* file_compressor::get_extension() const
{
    return extension_;
}

inline unsigned file_compressor::get_min_index() const
{
    return min_index_;
}

}

#endif
