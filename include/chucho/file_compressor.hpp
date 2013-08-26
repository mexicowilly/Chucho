/*
 * Copyright 2013 Will Mason
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

#include <chucho/export.hpp>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT file_compressor
{
public:
    virtual ~file_compressor() { }

    virtual void compress(const std::string& file_name) = 0;
    const char* get_extension() const;
    unsigned get_min_index() const;

protected:
    file_compressor(unsigned min_idx, const char* extension);

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
