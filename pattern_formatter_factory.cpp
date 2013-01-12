#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/pattern_formatter_memento.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>
#include <assert.h>

namespace chucho
{

pattern_formatter_factory::pattern_formatter_factory()
{
    set_status_origin("pattern_formatter_factory");
}

std::shared_ptr<configurable> pattern_formatter_factory::create_configurable(std::shared_ptr<memento> mnto)
{
    auto pfm = std::dynamic_pointer_cast<pattern_formatter_memento>(mnto);
    assert(pfm);
    if (pfm->get_pattern().empty())
        throw exception("pattern_formatter_factory: The pattern must be set");
    std::shared_ptr<configurable> cnf(new pattern_formatter(pfm->get_pattern()));
    report_info("Created a " + demangle::get_demangled_name(typeid(*cnf)));
    return cnf;
}

std::shared_ptr<memento> pattern_formatter_factory::create_memento(const configurator& cfg)
{
    std::shared_ptr<memento> mnto(new pattern_formatter_memento(cfg));
    return mnto;
}

}
