#include <chucho/writer_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

writer_memento::writer_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("writer_memento");
    set_handler("formatter_id", [this] (const std::string& name) { fmt_ = cfg_.get_formatter(name); });
}

void writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto fmt = std::dynamic_pointer_cast<formatter>(cnf);
    if (fmt)
    {
        fmt_ = fmt;
    }
    else
    {
        report_error("A writer cannot be constructed with a formatter of type " +
            demangle::get_demangled_name(typeid(*cnf)));
    }
}

}
