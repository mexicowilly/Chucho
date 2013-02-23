#if !defined(CHUCHO_CERR_WRITER_FACTORY_HPP__)
#define CHUCHO_CERR_WRITER_FACTORY_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_factory.hpp>

namespace chucho
{

class cerr_writer_factory : public writer_factory
{
public:
    cerr_writer_factory();

    virtual std::shared_ptr<configurable> create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
