#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_FACTORY_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class level_threshold_filter_factory : public identifiable_factory
{
public:
    level_threshold_filter_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
