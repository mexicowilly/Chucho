#include <chucho/file_writer_factory.hpp>
#include <chucho/file_writer_memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, file_writer_factory)

namespace chucho
{

file_writer_factory::file_writer_factory()
{
    set_status_origin("file_writer_factory");
}

named_configurable file_writer_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate_id(mnto);
    std::shared_ptr<configurable> cnf;
    auto fwm = std::dynamic_pointer_cast<file_writer_memento>(mnto);
    assert(fwm);
    if (!fwm->get_formatter())
        throw exception("file_writer_factory: The writer's formatter is not set");
    if (fwm->get_file_name().empty())
        throw exception("file_writer_factory: The file name is not set");
    if (fwm->get_on_start() && fwm->get_flush())
    {
        cnf.reset(new file_writer(fwm->get_formatter(),
                                  fwm->get_file_name(),
                                  *fwm->get_on_start(),
                                  *fwm->get_flush()));
    }
    else if (fwm->get_flush())
    {
        cnf.reset(new file_writer(fwm->get_formatter(),
                                  fwm->get_file_name(),
                                  file_writer::on_start::APPEND,
                                  *fwm->get_flush()));
    }
    else if (fwm->get_on_start())
    {
        cnf.reset(new file_writer(fwm->get_formatter(),
                                  fwm->get_file_name(),
                                  *fwm->get_on_start()));
    }
    else
    {
        cnf.reset(new file_writer(fwm->get_formatter(),
                                  fwm->get_file_name()));
    }
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> file_writer_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new file_writer_memento(cfg));
    return mnto;
}

}
