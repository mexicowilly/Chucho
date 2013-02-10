#if !defined(CHUCHO_FILTER_HPP__)
#define CHUCHO_FILTER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>

namespace chucho
{

/**
 * @defgroup filters 
 * Filters are attached to @ref writers, and they can be used to 
 * refine log filtering beyond the basic level check. 
 */

/**
 * @class filter filter.hpp chucho/filter.hpp 
 * Conditionally filter an event. A writer may have a set of 
 * filters that provide finer-grained control of log output. 
 * Filter evaluation results in one of three values @ref result
 * "DENY", @ref result "NEUTRAL", or @ref result "ACCEPT". If 
 * the evaluation of all of a writer's filters results in @ref 
 * result "NEUTRAL" or @ref result "ACCEPT", then the event is 
 * allowed to be written. 
 *  
 * @ingroup filters 
 */
class CHUCHO_EXPORT filter : public status_reporter,
                             public configurable
{
public:
    /**
     * The result of filter evaluation.
     */
    enum class result
    {
        /**
         * The event is denied. This value prevents subsequent filters 
         * from being evaluated. It is a vetoing result.
         */
        DENY,
        /**
         * The event may pass. Subsequent filters are evaluated, whose 
         * results may affect the ultimate outcome. 
         */
        NEUTRAL,
        /**
         * The event is accepted. No other filters will be evaluated. 
         * The acceptance is unconditional. 
         */
        ACCEPT
    };

    /**
     * Evaluate the filter. Determine how this filter feels about 
     * this event. 
     * 
     * @param evt the event to evaluate
     * @return result the level of acceptance
     */
    virtual result evaluate(const event& evt) = 0;
};

}

#endif
