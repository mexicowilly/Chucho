#include <chucho/writer_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

writer_memento::writer_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("writer_memento");
    set_handler("formatter_id", [this] (const std::string& name) { fmt_ = cfg_.get_formatter(name); });
    set_handler("filter_id", [this] (const std::string& name) { filters_.push_back(cfg_.get_filter(name)); });
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
        auto flt = std::dynamic_pointer_cast<filter>(cnf);
        if (flt)
        {
            filters_.push_back(flt);
        }
        else
        {
            report_error("A writer cannot be constructed with a type of " +
                demangle::get_demangled_name(typeid(*cnf)));
        }
    }
}

}
