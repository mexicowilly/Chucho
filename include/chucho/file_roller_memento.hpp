/*
 * Copyright 2013-2018 Will Mason
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

#if !defined(CHUCHO_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_FILE_ROLLER_MEMENTO_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/memento.hpp>
#include <chucho/file_compressor.hpp>

namespace chucho
{

/**
 * @class file_roller_memento file_roller_memento.hpp chucho/file_roller_memento.hpp 
 * A @ref memento shared by all @ref file_roller classes. This 
 * @ref memento takes care of the @ref file_compressor that may 
 * appear in the configuration. All @ref file_roller memento 
 * classes should inherit from this @ref memento.
 * 
 * @ingroup rolling
 */
class CHUCHO_EXPORT file_roller_memento : public memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a memento.
     * 
     * @param cfg the configurator that is perform configuration
     */
    file_roller_memento(configurator& cfg);

    /**
     * Return the @ref file_compressor that may have been found by 
     * the configuration. 
     * 
     * @return the @ref file_compressor
     */
    std::unique_ptr<file_compressor> get_file_compressor();
    /**
     * Handle only the case of a @ref file_compressor. If other 
     * types can be handled by subclasses, then this method should 
     * be called. 
     * 
     * @param cnf the configurable to handle
     */
    virtual void handle(std::unique_ptr<configurable>&& cnf) override;

private:
    std::unique_ptr<file_compressor> compressor_;
};

inline std::unique_ptr<file_compressor> file_roller_memento::get_file_compressor()
{
    return std::move(compressor_);
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
