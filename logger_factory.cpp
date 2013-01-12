#include <chucho/logger_factory.hpp>
#include <chucho/logger_memento.hpp>
#include <chucho/logger.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

logger_factory::logger_factory()
{
    set_status_origin("logger_factory");
}

std::shared_ptr<configurable> logger_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto lm = std::dynamic_pointer_cast<logger_memento>(mnto);
    assert(lm);
    if (!lm->get_name())
        throw exception("logger_memento: The logger's name must be set");
    auto lgr = logger::get(*lm->get_name());
    if (lm->get_level())
        lgr->set_level(lm->get_level());
    std::for_each(lm->get_writers().begin(),
                  lm->get_writers().end(),
                  [&] (std::shared_ptr<writer> w) { lgr->add_writer(w); });
    if (lm->get_writes_to_ancestors())
        lgr->set_writes_to_ancestors(*lm->get_writes_to_ancestors());
    std::shared_ptr<configurable> cnf = std::static_pointer_cast<configurable>(lgr);
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)) + " named " + lgr->get_name());
    return cnf;
}

std::shared_ptr<memento> logger_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new logger_memento(cfg));
    return mnto;
}

}
