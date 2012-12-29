#if !defined(CHUCHO_FILE_WRITER_FACTORY_HPP__)
#define CHUCHO_FILE_WRITER_FACTORY_HPP__

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class file_writer_factory : public identifiable_factory
{
public:
    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
