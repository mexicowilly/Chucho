#include <chucho/yaml_configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/configurable_factory.hpp>

namespace
{

}

namespace chucho
{

yaml_configurator::yaml_configurator()
{
    set_status_origin("yaml_configurator");
}

void yaml_configurator::configure(std::istream& in)
{
    try
    {
        YAML::Parser prs(in);
        YAML::Node doc;
        while (prs.GetNextDocument(doc))
        {
            for (YAML::Iterator i = doc.begin(); i != doc.end(); i++)
            {
                std::string type;
                try
                {
                    i.first() >> type;
                    type = resolve_variables(type);
                }
                catch (YAML::Exception& e)
                {
                    std::throw_with_nested(exception("Every top-level YAML node must be a map"));
                }
                if (type == "variables")
                {
                    add_variables(extract_variables(i.second()));
                }
                else
                {
                    auto found = get_factories().find(type);
                    if (found == get_factories().end())
                    {
                        report_error("The type " + type + " has no registered factory");
                    }
                    else
                    {
                        std::shared_ptr<memento> mnto = found->second->create_memento(*this);
                        handle(type, i.second(), mnto);
                        configurator::handle(found->second->create_configurable(mnto));
                    }
                }
            }
        }
    }
    catch (exception& e)
    {
        report_error("Error parsing YAML configuration: " + exception::nested_whats(e));
    }
}

std::map<std::string, std::string> yaml_configurator::extract_variables(const YAML::Node& n)
{
    std::map<std::string, std::string> result;
    for (YAML::Iterator i = n.begin(); i != n.end(); i++)
    {
        try
        {
            result[i.first().to<std::string>()] = i.second().to<std::string>();
        }
        catch (YAML::Exception& e)
        {
            report_error(std::string("Invalid map of variable key to value: ") + e.what());
        }
    }
    return result;
}

void yaml_configurator::handle(const std::string& key,
                               const YAML::Node& n,
                               std::shared_ptr<memento> mnto)
{
    YAML::Iterator i;
    if (n.Type() == YAML::NodeType::Scalar)
    {
        mnto->handle(key, resolve_variables(n.to<std::string>()));
    }
    else if (n.Type() == YAML::NodeType::Map)
    {
        i = n.begin();
        if (i.second().Type() == YAML::NodeType::Scalar)
        {
            handle(resolve_variables(i.first().to<std::string>()), i.second(), mnto);
        }
        else if (i.second().Type() == YAML::NodeType::Map)
        {
            std::string first = resolve_variables(i.first().to<std::string>());
            auto found = get_factories().find(first);
            if (found == get_factories().end())
            {
                handle(first, i.second(), mnto);
            }
            else
            {
                std::shared_ptr<memento> sub = found->second->create_memento(*this);
                handle(first, i.second(), sub);
                configurator::handle(found->second->create_configurable(sub));
            }
        }
        else
        {
            report_error("An illegal YAML sequence was found in " + key);
        }
    }
    else if (n.Type() == YAML::NodeType::Sequence)
    {
        for (i = n.begin(); i != n.end(); i++)
            handle("", *i, mnto);
    }
}

}
