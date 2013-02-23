#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_FACTORY_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_FACTORY_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class numbered_file_roller_factory : public configurable_factory
{
public:
    numbered_file_roller_factory();

    virtual std::shared_ptr<configurable> create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
