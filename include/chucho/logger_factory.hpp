#if !defined(CHUCHO_LOGGER_FACTORY_HPP__)
#define CHUCHO_LOGGER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class logger_factory : public configurable_factory
{
public:
    logger_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
