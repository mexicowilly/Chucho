#if !defined(CHUCHO_PATTERN_FORMATTER_FACTORY_HPP__)
#define CHUCHO_PATTERN_FORMATTER_FACTORY_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class pattern_formatter_factory : public configurable_factory
{
public:
    pattern_formatter_factory();

    virtual std::shared_ptr<configurable> create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
