#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/level_threshold_filter_memento.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

level_threshold_filter_factory::level_threshold_filter_factory()
{
    set_status_origin("level_threshold_filter_factory");
}

std::shared_ptr<configurable> level_threshold_filter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    assert(dynamic_cast<level_threshold_filter_memento*>(mnto.get()));
    auto ltfm = std::dynamic_pointer_cast<level_threshold_filter_memento>(mnto);
    assert(ltfm);
    if (!ltfm->get_level())
        throw exception("level_threshold_filter_factory: The level must be set");
    std::shared_ptr<configurable> cnf(new level_threshold_filter(ltfm->get_level()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> level_threshold_filter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new level_threshold_filter_memento(cfg));
    return mnto;
}

}
