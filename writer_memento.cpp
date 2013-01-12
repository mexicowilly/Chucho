#include <chucho/writer_memento.hpp>
#include <chucho/demangle.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

writer_memento::writer_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("writer_memento");
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
            throw exception("A writer cannot be constructed with a type of " +
                demangle::get_demangled_name(typeid(*cnf)));
        }
    }
}

}
