#include <chucho/logger_factory.hpp>
#include <chucho/logger_memento.hpp>
#include <chucho/logger.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, logger_factory)

namespace chucho
{

named_configurable logger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    assert(dynamic_cast<logger_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf = std::dynamic_pointer_cast<configurable>(
        logger::get(*static_cast<logger_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> logger_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new logger_memento(cfg));
    return mnto;
}

}
