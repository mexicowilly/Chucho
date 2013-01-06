#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cerr_writer_memento.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, cerr_writer_factory)

namespace chucho
{

cerr_writer_factory::cerr_writer_factory()
{
    set_status_origin("cerr_writer_factory");
}

named_configurable cerr_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto cwm = std::dynamic_pointer_cast<cerr_writer_memento>(mnto);
    assert(cwm);
    std::shared_ptr<configurable> cnf(new cerr_writer(cwm->get_formatter()));
    set_filters(cnf, cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> cerr_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cerr_writer_memento(cfg));
    return mnto;
}

}
