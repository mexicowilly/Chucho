#include <chucho/cout_writer_factory.hpp>
#include <chucho/cout_writer_memento.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

cout_writer_factory::cout_writer_factory()
{
    set_status_origin("cout_writer_factory");
}

std::shared_ptr<configurable> cout_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto cwm = std::dynamic_pointer_cast<cout_writer_memento>(mnto);
    assert(cwm);
    std::shared_ptr<configurable> cnf(new cout_writer(cwm->get_formatter()));
    set_filters(cnf, cwm);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> cout_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new cout_writer_memento(cfg));
    return mnto;
}

}

