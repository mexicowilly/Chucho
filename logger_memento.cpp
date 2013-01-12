#include <chucho/logger_memento.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

logger_memento::logger_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("logger_memento");
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(name); });
    set_handler("name", [this] (const std::string& name) {  name_ = name; });
    set_handler("writes_to_ancestors", [this] (const std::string& name) { writes_to_ancestors_ = boolean_value(name); });
}

void logger_memento::handle(std::shared_ptr<configurable> cnf)
{
    auto wrt = std::dynamic_pointer_cast<writer>(cnf);
    if (wrt)
    {
        writers_.push_back(wrt);
    }
    else
    {
        report_error("A logger cannot make use of type " +
            demangle::get_demangled_name(typeid(*cnf)) + ". Only writers can be embedded in the configuration.");
    }
}

}
