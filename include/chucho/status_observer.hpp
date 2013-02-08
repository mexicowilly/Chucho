#if !defined(CHUCHO_STATUS_OBSERVER_HPP__)
#define CHUCHO_STATUS_OBSERVER_HPP__

#include <chucho/status.hpp>

namespace chucho
{

/**
 * @class status_observer status_observer.hpp chucho/status_observer.hpp
 * A status_observer becomes aware of each @ref status event. 
 * Chucho reports its interal activities via @ref status 
 * messages. These can be used to diagnose a problem with 
 * configuration. You may register an observer with the @ref 
 * status_manager in order to get immediate notification of 
 * @ref status events. 
 *  
 * @ingroup status 
 */
class CHUCHO_EXPORT status_observer
{
public:
    /**
     * @name Destructor
     */
    //@{
    /**
     * Destroy a status_observer.
     */
    virtual ~status_observer();
    //@}

    /**
     * Become aware of a new @ref status event. This method is 
     * invoked once for each observer and event.
     * 
     * @param st the event
     */
    virtual void status_reported(const status& st) = 0;
};

}

#endif
