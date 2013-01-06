#if !defined(CHUCHO_COUT_WRITER_FACTORY_HPP__)
#define CHUCHO_COUT_WRITER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_factory.hpp>

namespace chucho
{

class cout_writer_factory : public writer_factory
{
public:
    cout_writer_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
