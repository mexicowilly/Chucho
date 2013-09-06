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

#if !defined(CHUCHO_FILE_COMPRESSOR_FACTORY_HPP__)
#define CHUCHO_FILE_COMPRESSOR_FACTORY_HPP__

#include <chucho/configurable_factory.hpp>

namespace chucho
{

/**
 * @class file_compressor_factory file_compressor_factory.hpp chucho/file_compressor_factory.hpp 
 * A factory for creating a @ref file_compressor at 
 * configuration time. This factory should be subclassed for 
 * specific compressor types. The overriden @ref create_memento 
 * method, however, should serve all subclasses, as it always 
 * returns a @ref file_compressor_memento. 
 * 
 * @ingroup compressors
 */
class CHUCHO_EXPORT file_compressor_factory : public configurable_factory
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a factory.
     */
    file_compressor_factory();
    //@}

    /**
     * Create the @ref file_compressor_memento. This method always 
     * returns an instance of @ref file_compressor_memento, and 
     * there should be no need to override this method in 
     * subclasses. 
     * 
     * @param cfg the @ref configuration performing configuration
     * @return a @ref file_compressor_memento
     */
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
