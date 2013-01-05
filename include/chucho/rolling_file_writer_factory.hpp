#if !defined(CHUCHO_ROLLING_FILE_WRITER_FACTORY_HPP__)
#define CHUCHO_ROLLING_FILE_WRITER_FACTORY_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/identifiable_factory.hpp>

namespace chucho
{

class rolling_file_writer_factory : public identifiable_factory
{
public:
    rolling_file_writer_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) override;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) override;
};

}

#endif
