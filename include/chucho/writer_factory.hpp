#if !defined(CHUCHO_WRITER_FACTORY_HPP__)
#define CHUCHO_WRITER_FACTORY_HPP__

#include <chucho/configurable_factory.hpp>
#include <chucho/writer_memento.hpp>

namespace chucho
{

/**
 * @class writer_factory writer_factory.hpp chucho/writer_factory.hpp
 * A @ref configurable_factory that has common functionality for 
 * writers. All writers can have filters that are discovered at 
 * configuration time. This class provides a helper method to 
 * facilitate setting a writer's filters based on its memento. 
 *  
 * @ingroup configuration 
 */
class CHUCHO_EXPORT writer_factory : public configurable_factory
{
protected:
    /**
     * Set a writer's filters. The @ref configurable here must be a 
     * @ref writer. It's filters are extracted from the @ref memento 
     * and set appropriately.
     * 
     * @param cnf the @ref writer that wants filters
     * @param mnto the @ref memento that has the filters
     */
    void set_filters(std::shared_ptr<configurable> cnf, std::shared_ptr<writer_memento> mnto);
};

}

#endif
