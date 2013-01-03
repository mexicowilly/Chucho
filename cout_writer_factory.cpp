#include <chucho/cout_writer_factory.hpp>
#include <chucho/cout_writer_memento.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, cout_writer_factory)

namespace chucho
{

cout_writer_factory::cout_writer_factory()
{
    set_status_origin("cout_writer_factory");
}

named_configurable cout_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate_id(mnto);
    assert(dynamic_cast<cout_writer_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf(new cout_writer(*static_cast<cout_writer_memento*>(mnto.get())));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> cout_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cout_writer_memento(cfg));
    return mnto;
}

}

