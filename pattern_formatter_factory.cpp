#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/pattern_formatter_memento.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/exception.hpp>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, pattern_formatter_factory)

namespace chucho
{

named_configurable pattern_formatter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    if (mnto->get_id().empty())
        throw exception("Formatters must have an id key");
    std::shared_ptr<configurable> cnf(new pattern_formatter(*static_cast<pattern_formatter_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> pattern_formatter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new pattern_formatter_memento(cfg));
    return mnto;
}

}
