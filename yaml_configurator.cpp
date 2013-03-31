/*
 * Copyright 2013 Will Mason
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

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
                bool is_map = false;
                try
                {
                    // If the iterator can be dereferenced, then it's not is not a map. But the
                    // dereferenced node must claim to be a map. Yeah, it took me a while to
                    // figure this out...
                    if (i->Type() != YAML::NodeType::Map)
                        throw exception("The YAML configuration only supports maps or sequences of maps at the top level");
                }
                catch (YAML::Exception&)
                {
                    is_map = true;
                }
                const YAML::Node& first = is_map ? i.first() : i->begin().first();
                const YAML::Node& second = is_map ? i.second() : i->begin().second();
                try
                {
                    std::string type = resolve_variables(first.to<std::string>());
                    if (type == "variables")
                    {
                        add_variables(extract_variables(second));
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
                            handle(type, second, mnto);
                            found->second->create_configurable(mnto);
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
                    std::throw_with_nested(yaml_location_exception(first));
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
        if (n.Type() == YAML::NodeType::Sequence &&
            i->Type() != YAML::NodeType::Map)
        {
            throw exception("Only sequences of maps are supported when defining variables");
        }
        const YAML::Node& first = (n.Type() == YAML::NodeType::Sequence) ? i->begin().first() : i.first();
        const YAML::Node& second = (n.Type() == YAML::NodeType::Sequence) ? i->begin().second() : i.second();
        try
        {
            result[first.to<std::string>()] = second.to<std::string>();
        }
        catch (YAML::Exception& e)
        {
            try
            {
                throw exception(std::string("Invalid map of variable key to value: ") + e.what());
            }
            catch (exception&)
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
    std::string val;
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
                if (n.Type() == YAML::NodeType::Sequence)
                {
                    if (i->Type() == YAML::NodeType::Scalar)
                    {
                        val = resolve_variables(i->to<std::string>());
                        auto found = get_factories().find(val);
                        if (found == get_factories().end())
                            throw exception("The type " + val + " has no registered factory");
                        else
                            mnto->handle(found->second->create_configurable(found->second->create_memento(*this)));
                        continue;
                    }
                    if (i->Type() != YAML::NodeType::Map)
                        throw exception("Only sequences of maps or scalars are supported in this configuration");
                }
                const YAML::Node& first = (n.Type() == YAML::NodeType::Sequence) ? i->begin().first() : i.first();
                const YAML::Node& second = (n.Type() == YAML::NodeType::Sequence) ? i->begin().second() : i.second();
                if (second.Type() == YAML::NodeType::Scalar)
                {
                    handle(resolve_variables(first.to<std::string>()), second, mnto);
                }
                else
                {
                    val = resolve_variables(first.to<std::string>());
                    auto found = get_factories().find(val);
                    if (found == get_factories().end())
                    {
                        handle(val, second, mnto);
                    }
                    else
                    {
                        try
                        {
                            std::shared_ptr<memento> sub = found->second->create_memento(*this);
                            handle(val, second, sub);
                            mnto->handle(found->second->create_configurable(sub));
                        }
                        catch (yaml_location_exception&)
                        {
                            // It already has location information
                            throw;
                        }
                        catch (std::exception&)
                        {
                            std::throw_with_nested(yaml_location_exception(second));
                        }
                    }
                }
            }
        }
    }
    catch (yaml_location_exception&)
    {
        // It already has location information
        throw;
    }
    catch (std::exception&)
    {
        std::throw_with_nested(yaml_location_exception(n));
    }
}

}
