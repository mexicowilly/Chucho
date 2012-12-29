#include <chucho/configurator.hpp>
#include <chucho/exception.hpp>
#include <regex>

namespace chucho
{

configurator::configurator()
{
    set_status_origin("configurator");
}

std::map<std::string, std::shared_ptr<configurable_factory>>& configurator::get_factories()
{
    static std::map<std::string, std::shared_ptr<configurable_factory>> factories;

    return factories;
}

std::shared_ptr<formatter> configurator::get_formatter(const std::string& name) const
{
    auto found = formatters_.find(name);
    if (found == formatters_.end())
        throw exception("The formatter " + name + " was not found in this configuration");
    return found->second;
}

std::shared_ptr<writer> configurator::get_writer(const std::string& name) const
{
    auto found = writers_.find(name);
    if (found == writers_.end())
        throw exception("The writer " + name + " was not found in this configuration");
    return found->second;
}

void configurator::handle(const named_configurable& cnf)
{
    std::shared_ptr<formatter> fmt = std::dynamic_pointer_cast<formatter>(cnf.get_configurable());
    if (fmt)
    {
        if (cnf.get_name().empty())
            report_error("The formatter has no id, so it cannot be used by writers");
        else
            formatters_[cnf.get_name()] = fmt;
        return;
    }
    std::shared_ptr<writer> wrt = std::dynamic_pointer_cast<writer>(cnf.get_configurable());
    if (wrt)
    {
        if (cnf.get_name().empty())
            report_error("The writer has no id, so it cannot be used by loggers");
        else
            writers_[cnf.get_name()] = wrt;
    }
}

std::string configurator::resolve_variables(const std::string& val)
{
    static std::regex re("\\$\\{(.+?)\\}");

    std::string result(val);
    int pos_offset = 0;
    auto itor = std::sregex_iterator(val.begin(), val.end(), re);
    auto end = std::sregex_iterator();
    while (itor != end)
    {
        const std::smatch& sm(*itor);
        auto found = variables_.find(sm[1]);
        if (found != variables_.end())
        {
            result.replace(sm.position() + pos_offset, sm.length(), found->second);
            pos_offset += found->second.length() - sm.length();
        }
        ++itor;
    }
    return result;
}

}
