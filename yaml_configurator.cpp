#include <chucho/yaml_configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/configurable_factory.hpp>

namespace
{

class yaml_location_exception : public chucho::exception
{

public:
    yaml_location_exception(const YAML::Node& node);
};

yaml_location_exception::yaml_location_exception(const YAML::Node& node)
    : exception("")
{
    YAML::Mark mark = node.GetMark();
    message_ = "[line " + std::to_string(mark.line) + ", column " +
        std::to_string(mark.column) + "]";
}

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
                try
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
                            throw exception("The type " + type + " has no registered factory");
                        }
                        else
                        {
                            std::shared_ptr<memento> mnto = found->second->create_memento(*this);
                            handle(type, i.second(), mnto);
                            configurator::handle(found->second->create_configurable(mnto));
                        }
                    }
                }
                catch (yaml_location_exception& yle)
                {
                    // it already has the location information
                    throw;
                }
                catch (std::exception& se)
                {
                    std::throw_with_nested(yaml_location_exception(*i));
                }
            }
        }
    }
    catch (std::exception& e)
    {
        report_error("Error parsing the YAML configuration");
        report_error(exception::nested_whats(e));
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
            try
            {
                throw exception(std::string("Invalid map of variable key to value: ") + e.what());
            }
            catch (exception& e2)
            {
                std::throw_with_nested(yaml_location_exception(*i));
            }
        }
    }
    return result;
}

void yaml_configurator::handle(const std::string& key,
                               const YAML::Node& n,
                               std::shared_ptr<memento> mnto)
{
    try
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
                    try
                    {
                        std::shared_ptr<memento> sub = found->second->create_memento(*this);
                        handle(first, i.second(), sub);
                        auto cnf = found->second->create_configurable(sub);
                        mnto->handle(cnf.get_configurable());
                    }
                    catch (std::exception& e)
                    {
                        std::throw_with_nested(yaml_location_exception(i.second()));
                    }
                }
            }
            else
            {
                throw exception("An illegal YAML sequence was found in " + key);
            }
        }
        else if (n.Type() == YAML::NodeType::Sequence)
        {
            for (i = n.begin(); i != n.end(); i++)
                handle("", *i, mnto);
        }
    }
    catch (yaml_location_exception& yle)
    {
        // It already has location information
        throw;
    }
    catch (std::exception& se)
    {
        std::throw_with_nested(yaml_location_exception(n));
    }
}

}
