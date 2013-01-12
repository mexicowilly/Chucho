#include <chucho/yaml_configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/logger_factory.hpp>

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
    message_ = "YAML [line " + std::to_string(mark.line + 1) + ", column " +
        std::to_string(mark.column + 1) + "]";
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
                            if (dynamic_cast<logger_factory*>(found->second.get()) == 0)
                            {
                                if (!i.second().IsAliased())
                                {
                                    report_error("The top-level type " + type +
                                        " is not referenced elsewhere in configuration, so it is ignored");
                                }
                            }
                            else
                            {
                                std::shared_ptr<memento> mnto = found->second->create_memento(*this);
                                handle(type, i.second(), mnto);
                                found->second->create_configurable(mnto);
                            }
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
                    std::throw_with_nested(yaml_location_exception(i.first()));
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
        if (n.Type() == YAML::NodeType::Scalar)
        {
            mnto->handle(key, resolve_variables(n.to<std::string>()));
        }
        else
        {
            for (YAML::Iterator i = n.begin(); i != n.end(); i++)
            {
                if (n.Type() == YAML::NodeType::Sequence &&
                    i->Type() != YAML::NodeType::Map)
                {
                    throw exception("Only sequences of maps are supported in this configuration");
                }
                const YAML::Node& first = (n.Type() == YAML::NodeType::Sequence) ? i->begin().first() : i.first();
                const YAML::Node& second = (n.Type() == YAML::NodeType::Sequence) ? i->begin().second() : i.second();
                if (second.Type() == YAML::NodeType::Scalar)
                {
                    handle(resolve_variables(first.to<std::string>()), second, mnto);
                }
                else
                {
                    std::string key = resolve_variables(first.to<std::string>());
                    auto found = get_factories().find(key);
                    if (found == get_factories().end())
                    {
                        handle(key, second, mnto);
                    }
                    else
                    {
                        try
                        {
                            std::shared_ptr<memento> sub = found->second->create_memento(*this);
                            handle(key, second, sub);
                            mnto->handle(found->second->create_configurable(sub));
                        }
                        catch (std::exception& e)
                        {
                            std::throw_with_nested(yaml_location_exception(second));
                        }
                    }
                }
            }
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
