#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cerr_writer_memento.hpp>
#include <chucho/cerr_writer.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, cerr_writer_factory)

namespace chucho
{

named_configurable cerr_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate(mnto);
    assert(dynamic_cast<cerr_writer_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf(new cerr_writer(*static_cast<cerr_writer_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> cerr_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cerr_writer_memento(cfg));
    return mnto;
}

}
