#if !defined(CHUCHO_LOGGER_FACTORY_HPP__)
#define CHUCHO_LOGGER_FACTORY_HPP__

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class CHUCHO_EXPORT logger_factory : public configurable_factory
{
public:
    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
