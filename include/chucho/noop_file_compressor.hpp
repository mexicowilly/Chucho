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

#if !defined(CHUCHO_NOOP_FILE_COMPRESSOR_HPP__)
#define CHUCHO_NOOP_FILE_COMPRESSOR_HPP__

#include <chucho/file_compressor.hpp>

namespace chucho
{

/**
 * @class noop_file_compressor noop_file_compressor.hpp chucho/noop_file_compressor.hpp 
 * Don't do anything. The noop_file_compressor is used during 
 * configuration. If a @ref file_compressor is requested in a 
 * configuration when the Chucho running the configuration does 
 * not have support for the named compressor, then a 
 * noop_file_compressor is substituted. This is to support cases 
 * where a single configuration file is used across multiple 
 * deployments with possibly differing build options for Chucho.
 * 
 * @ingroup compressors
 */
class CHUCHO_EXPORT noop_file_compressor : public file_compressor
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a compressor.
     */
    noop_file_compressor();
    //@}

    /**
     * Do nothing.
     * 
     * @param file_name the name of the file for which to do nothing
     */
    virtual void compress(const std::string& file_name) override;
};

}

#endif
