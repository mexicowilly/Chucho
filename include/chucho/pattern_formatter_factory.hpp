#if !defined(CHUCHO_PATTERN_FORMATTER_FACTORY_HPP__)
#define CHUCHO_PATTERN_FORMATTER_FACTORY_HPP__

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class pattern_formatter_factory : public identifiable_factory
{
public:
    pattern_formatter_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
