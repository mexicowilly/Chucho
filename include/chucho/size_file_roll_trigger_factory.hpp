#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurable_factory.hpp>

namespace chucho
{

class size_file_roll_trigger_factory : public configurable_factory
{
public:
    size_file_roll_trigger_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
