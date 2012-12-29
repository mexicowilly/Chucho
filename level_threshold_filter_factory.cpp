#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/level_threshold_filter_memento.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <assert.h>

CHUCHO_REGISTER_CONFIGURABLE_FACTORY(chucho, level_threshold_filter_factory)

namespace chucho
{

named_configurable level_threshold_filter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    validate(mnto);
    assert(dynamic_cast<level_threshold_filter_memento*>(mnto.get()));
    std::shared_ptr<configurable> cnf(new level_threshold_filter(*static_cast<level_threshold_filter_memento*>(mnto.get())));
    return named_configurable(mnto->get_id(), cnf);
}

std::shared_ptr<memento> level_threshold_filter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new level_threshold_filter_memento(cfg));
    return mnto;
}

}
