#if !defined(CHUCHO_CONFIGURABLE_HPP__)
#define CHUCHO_CONFIGURABLE_HPP__

#include <chucho/export.hpp>

namespace chucho
{

/**
 * Indicates configurability of an object. A class that inherits 
 * from configurable can be configured via a chucho 
 * configuration file. Such a class will have a corresponding 
 * @ref memento and @ref configurable_factory class that can be 
 * used to perform configuration. Before configuration occurs, 
 * an instance of the corresponding factory class must be added 
 * to the configurator via the 
 * @ref configurator::add_configurable_factory() method. For the 
 * factories associated with built-in chucho classes, this is 
 * handled automatically. 
 */
class CHUCHO_EXPORT configurable
{
public:
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy this configurable.
     */
    virtual ~configurable();
    //@}
};

}

#endif
