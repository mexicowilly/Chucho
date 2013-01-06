#include <chucho/rolling_file_writer_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

rolling_file_writer_memento::rolling_file_writer_memento(const configurator& cfg)
    : file_writer_memento(cfg)
{
    set_status_origin("rolling_file_writer_memento");
}

void rolling_file_writer_memento::handle(std::shared_ptr<configurable> cnf)
{
    file_writer_memento::handle(cnf);
    auto roller = std::dynamic_pointer_cast<file_roller>(cnf);
    if (roller)
    {
        roller_ = roller;
    }
    else
    {
        auto trigger = std::dynamic_pointer_cast<file_roll_trigger>(cnf);
        if (trigger)
            trigger_ = trigger;
        else
            report_error("A file_writer cannot be constructed with a file_roller or a file_roll_trigger of type " +
                demangle::get_demangled_name(typeid(*cnf)));
    }
}

}
