#if !defined(CHUCHO_COUT_WRITER_FACTORY_HPP__)
#define CHUCHO_COUT_WRITER_FACTORY_HPP__

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class CHUCHO_EXPORT cout_writer_factory : public identifiable_factory
{
public:
    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
