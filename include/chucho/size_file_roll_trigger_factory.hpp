#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_FACTORY_HPP__

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class size_file_roll_trigger_factory : public identifiable_factory
{
public:
    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
