#include <chucho/writer_memento.hpp>

namespace chucho
{

writer_memento::writer_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("writer_memento");
    set_handler("formatter-id", [this] (const std::string& name) { fmt_ = cfg_.get_formatter(name); });
}

}
