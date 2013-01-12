#if !defined(CHUCHO_FILE_WRITER_FACTORY_HPP__)
#define CHUCHO_FILE_WRITER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_factory.hpp>

namespace chucho
{

class file_writer_factory : public writer_factory
{
public:
    file_writer_factory();

    virtual std::shared_ptr<configurable> create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
