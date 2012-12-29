#include <chucho/cout_writer_factory.hpp>
#include <chucho/cout_writer_memento.hpp>
#include <chucho/cout_writer.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, cout_writer_factory)

namespace chucho
{

named_configurable cout_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate(mnto);
    assert(dynamic_cast<cout_writer_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf(new cout_writer(*static_cast<cout_writer_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> cout_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cout_writer_memento(cfg));
    return mnto;
}

}

