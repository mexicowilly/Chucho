#ifndef CHUCHO_FILTER_MEMENTO_HPP__
#define CHUCHO_FILTER_MEMENTO_HPP__

#include <chucho/memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT filter_memento : public memento
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
    /**
     * Return the name of the writer.
     *
     * @return the name
     */
    const std::string& get_name() const;

private:
    std::string name_;
};

inline const std::string& filter_memento::get_name() const
{
    return name_;
}

}

#endif
