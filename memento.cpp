#include <chucho/memento.hpp>
#include <chucho/exception.hpp>
#include <chucho/demangle.hpp>

namespace chucho
{

memento::memento(const configurator& cfg)
    : cfg_(cfg)
{
    set_status_origin("memento");
}

bool memento::boolean_value(const std::string& value)
{
    static const std::string zero("0");
    static const std::string falso("false");

    std::string low = to_lower(value);
    return (low.empty() || low == zero || low == falso) ? false : true;
}

void memento::handle(const std::string& key, const std::string& value)
{
    auto found = handlers_.find(key);
    if (found == handlers_.end())
        report_error("The key " + key + " is not supported with " + get_status_origin());
    else
        found->second(value);
}

void memento::handle(std::shared_ptr<configurable> cnf)
{
    throw exception(demangle::get_demangled_name(typeid(*this)) + " cannot handle a type of " +
        demangle::get_demangled_name(typeid(*cnf)));
}

std::string memento::to_lower(const std::string& value)
{
    std::string low;
    std::transform(value.begin(),
                   value.end(),
                   std::back_inserter(low),
                   (int(*)(int))std::tolower);
    return low;
}

}
