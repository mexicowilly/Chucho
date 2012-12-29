#include <chucho/file_writer_factory.hpp>
#include <chucho/file_writer_memento.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, file_writer_factory)

namespace chucho
{

named_configurable file_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate(mnto);
    assert(dynamic_cast<file_writer_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf(new file_writer(*static_cast<file_writer_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> file_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new file_writer_memento(cfg));
    return mnto;
}

}
