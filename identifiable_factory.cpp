#include <chucho/identifiable_factory.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

void identifiable_factory::validate_id(std::shared_ptr<memento> mnto)
{
    if (mnto->get_id().empty())
        throw exception("There is no id key associated with this object");
}

}
