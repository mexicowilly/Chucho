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

#if !defined(CHUCHO_FILE_COMPRESSOR_MEMENTO_HPP_)
#define CHUCHO_FILE_COMPRESSOR_MEMENTO_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

/**
 * @class file_compressor_memento file_compressor_memento.hpp chucho/file_compressor_memento.hpp 
 * A @ref memento that handles shared functionality for all 
 * file_compressors. In fact, no file_compressor_memento 
 * subclasses are required, since this one takes care of the 
 * single constructor argument regarding the @ref 
 * file_compressor minimum index. 
 * 
 * @ingroup fcompressors
 */
class CHUCHO_EXPORT file_compressor_memento : public memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a file_compressor_memento.
     * 
     * @param cfg the configurator that is perform configuration
     */
    file_compressor_memento(configurator& cfg);
    //@}

    /**
     * Return the minimum index that has been discovered during 
     * configuration time. 
     * 
     * @return the minimum index
     */
    const optional<unsigned>& get_min_index() const;

private:
    optional<unsigned> min_index_;
};

inline const optional<unsigned>& file_compressor_memento::get_min_index() const
{
    return min_index_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
