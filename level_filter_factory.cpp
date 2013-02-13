#include <chucho/level_filter_factory.hpp>
#include <chucho/level_filter_memento.hpp>
#include <chucho/level_filter.hpp>
#include <chucho/exception.hpp>
#include <assert.h>

namespace chucho
{

level_filter_factory::level_filter_factory()
{
    set_status_origin("level_filter_factory");
}

std::shared_ptr<configurable> level_filter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    assert(dynamic_cast<level_filter_memento*>(mnto.get()));
    auto lfm = std::dynamic_pointer_cast<level_filter_memento>(mnto);
    assert(lfm);
    if (!lfm->get_level())
        throw exception("level_filter_factory: The level must be set");
    if (!lfm->get_on_match())
        throw exception("level_filter_factory: The on_match key must be set");
    if (!lfm->get_on_mismatch())
        throw exception("level_filter_factory: The on_mismatch key must be set");
    std::shared_ptr<configurable> cnf(new level_filter(lfm->get_level(),
                                                       *lfm->get_on_match(),
                                                       *lfm->get_on_mismatch()));
    return cnf;
}

std::shared_ptr<memento> level_filter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new level_filter_memento(cfg));
    return mnto;
}

}