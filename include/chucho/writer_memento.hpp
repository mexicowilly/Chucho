/*
 * Copyright 2013-2015 Will Mason
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

#if !defined(CHUCHO_WRITER_MEMENTO_HPP__)
#define CHUCHO_WRITER_MEMENTO_HPP__

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
#endif

#include <chucho/memento.hpp>
#include <chucho/filter.hpp>
#include <chucho/formatter.hpp>
#include <vector>

namespace chucho
{

/**
 * @class writer_memento writer_memento.hpp chucho/writer_memento.hpp
 * A @ref memento that has common functionality for writers. All 
 * writers share the need for a @ref formatter and optional @ref 
 * filters. 
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT writer_memento : public memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a writer_memento.
     * 
     * @param cfg the configurator that is perform configuration
     */
    writer_memento(configurator& cfg);
    //@}

    /**
     * Return the filters that have been discovered during 
     * configuration time. 
     * 
     * @return the filters
     */
    const std::vector<std::shared_ptr<filter>>& get_filters() const;
    /**
     * Return the formatter that has been discovered during 
     * configuration time. 
     * 
     * @return the formatter
     */
    std::shared_ptr<formatter> get_formatter() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<formatter> fmt_;
    std::vector<std::shared_ptr<filter>> filters_;
};

inline const std::vector<std::shared_ptr<filter>>& writer_memento::get_filters() const
{
    return filters_;
}

inline std::shared_ptr<formatter> writer_memento::get_formatter() const
{
    return fmt_;
}

}

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif
