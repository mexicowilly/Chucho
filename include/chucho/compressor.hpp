/*
 * Copyright 2013-2020 Will Mason
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

#if !defined(CHUCHO_COMPRESSOR_HPP__)
#define CHUCHO_COMPRESSOR_HPP__

#include <chucho/configurable.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/non_copyable.hpp>
#include <vector>

namespace chucho
{

/**
 * @class compressor compressor.hpp chucho/compressor.hpp
 * Compressors that can be used with instances of @ref message_queue_writer.
 * 
 * @ingroup compressors
 */
class CHUCHO_EXPORT compressor : non_copyable, public configurable, public status_reporter
{
public:
    /**
     * @name Destructor
     * @{
     */
    /**
     * Destroy a compressor.
     */
    virtual ~compressor() { }
    /**
     * @}
     */

    /**
     * Compress bytes.
     * @param in the bytes to compress
     * @return the compressed bytes
     */
    virtual std::vector<std::uint8_t> compress(const std::vector<std::uint8_t>& in) = 0;
};

}

#endif
