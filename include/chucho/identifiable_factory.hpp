#if !defined(CHUCHO_IDENTIFIABLE_FACTORY_HPP__)
#define CHUCHO_IDENTIFIABLE_FACTORY_HPP__

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class CHUCHO_EXPORT identifiable_factory : public configurable_factory
{
protected:
    void validate_id(std::shared_ptr<memento> mnto);
};

}

#endif
