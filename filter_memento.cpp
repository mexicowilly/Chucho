#include <chucho/filter_memento.hpp>

namespace chucho
{

filter_memento::filter_memento(configurator& cfg)
    : nameable_memento(cfg)
{
    set_status_origin("filter_memento");
}

}