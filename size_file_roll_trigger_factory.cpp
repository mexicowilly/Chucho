#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/size_file_roll_trigger_memento.hpp>
#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, size_file_roll_trigger_factory)

namespace chucho
{

size_file_roll_trigger_factory::size_file_roll_trigger_factory()
{
    set_status_origin("sile_file_roll_trigger_factory");
}

named_configurable size_file_roll_trigger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate_id(mnto);
    auto sfrtm = std::dynamic_pointer_cast<size_file_roll_trigger_memento>(mnto);
    assert(sfrtm);
    if (!sfrtm->get_max_size())
        throw exception("size_file_roll_trigger_factory: The max_size field must be set to create a size_file_roll_trigger");
    std::shared_ptr<configurable> cnf(new size_file_roll_trigger(*sfrtm->get_max_size()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> size_file_roll_trigger_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new size_file_roll_trigger_memento(cfg));
    return mnto;
}

}
