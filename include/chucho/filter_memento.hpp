#ifndef CHUCHO_FILTER_MEMENTO_HPP__
#define CHUCHO_FILTER_MEMENTO_HPP__

#include <chucho/nameable_memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT filter_memento : public nameable_memento
{
public:
    /**
     * @name Constructor
     */
    //@{
    /**
     * Construct a filter_memento.
     *
     * @param cfg the configurator that is perform configuration
     */
    filter_memento(configurator& cfg);
    //@}
};

}

#endif
