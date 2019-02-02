/*
 * Copyright 2013-2019 Will Mason
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
#include <chucho/configuration.hpp>
#include <chucho/yaml_parser.hpp>
#include <istream>

namespace
{

class yaml_location_exception : public chucho::exception
{
public:
    yaml_location_exception(const yaml_mark_t& mark, const std::string& msg)
        : exception("YAML error [line " + std::to_string(mark.line + 1) +
                    ", column " + std::to_string(mark.column + 1) + "] " + msg)
    {
    }
};

}

namespace chucho
{

yaml_configurator::yaml_configurator(security_policy& sec_pol)
    : configurator(sec_pol)
{
    set_status_origin("yaml_configurator");
}

void yaml_configurator::configure(std::istream& in)
{
    yaml_parser prs(in);
    yaml_document_t doc;
    while (true)
    {
        if (!yaml_parser_load(prs, &doc))
            throw yaml_location_exception(prs.problem_mark(), prs.problem_message());
        struct doc_sentry
        {
            doc_sentry(yaml_document_t& doc) : doc_(doc) { }
            ~doc_sentry() { yaml_document_delete(&doc_); }
            yaml_document_t& doc_;
        } ds(doc);
        // This pointer is cleaned when the document is destroyed
        yaml_node_t* node = yaml_document_get_root_node(&doc);
        if (node == nullptr)
            break;
        auto mnto = std::unique_ptr<memento>();
        handle(doc, *node, 1, std::string(), mnto);
    }
}

void yaml_configurator::extract_variables(yaml_document_t& doc, const yaml_node_t& node)
{
    std::map<std::string, std::string> vars;
    if (node.type == YAML_MAPPING_NODE)
    {
        for (yaml_node_pair_t* p = node.data.mapping.pairs.start;
             p < node.data.mapping.pairs.top;
             p++)
        {
            const yaml_node_t& key_node = *yaml_document_get_node(&doc, p->key);
            const yaml_node_t& value_node = *yaml_document_get_node(&doc, p->value);
            if (key_node.type == YAML_SCALAR_NODE && value_node.type == YAML_SCALAR_NODE)
            {
                vars[resolve_variables(reinterpret_cast<const char*>(key_node.data.scalar.value))] =
                    resolve_variables(reinterpret_cast<const char*>(value_node.data.scalar.value));
            }
        }
    }
    else if (node.type == YAML_SEQUENCE_NODE)
    {
        for (yaml_node_item_t* i = node.data.sequence.items.start;
             i < node.data.sequence.items.top;
             i++)
        {
            const yaml_node_t& item_node = *yaml_document_get_node(&doc, *i);
            if (item_node.type == YAML_MAPPING_NODE)
            {
                // There should only be one pair here
                for (yaml_node_pair_t* p = item_node.data.mapping.pairs.start;
                     p < item_node.data.mapping.pairs.top;
                     p++)
                {
                    const yaml_node_t& key_node = *yaml_document_get_node(&doc, p->key);
                    const yaml_node_t& value_node = *yaml_document_get_node(&doc, p->value);
                    if (key_node.type == YAML_SCALAR_NODE && value_node.type == YAML_SCALAR_NODE)
                    {
                        vars[resolve_variables(reinterpret_cast<const char*>(key_node.data.scalar.value))] =
                            resolve_variables(reinterpret_cast<const char*>(value_node.data.scalar.value));
                    }
                }
            }
        }
    }
    add_variables(vars);
}

void yaml_configurator::handle(yaml_document_t& doc,
                               const yaml_node_t& node,
                               int level,
                               const std::string& key,
                               std::unique_ptr<memento>& mnto)
{
    try
    {
        if (node.type == YAML_SCALAR_NODE)
        {
            std::string val(resolve_variables(reinterpret_cast<const char*>(node.data.scalar.value)));
            if (mnto)
            {
                auto found = get_factories().find(val);
                if (found == get_factories().end())
                {
                    if (key.empty()) 
                    {
                        report_warning("An incomplete mapping was found at line " + std::to_string(node.start_mark.line + 1) +
                            ", column " + std::to_string(node.start_mark.column + 1));
                    }
                    else
                    {
                        mnto->handle(key, val);
                    }
                }
                else
                {
                    auto mnto2 = found->second->create_memento(*this);
                    mnto->handle(std::move(found->second->create_configurable(mnto2)));
                }
            }
            else if (configuration::get_unknown_handler() && !key.empty())
            {
                configuration::get_unknown_handler()(key, val);
            }
        }
        else if (node.type == YAML_MAPPING_NODE)
        {
            for (yaml_node_pair_t* p = node.data.mapping.pairs.start;
                 p < node.data.mapping.pairs.top;
                 p++)
            {
                const yaml_node_t& key_node = *yaml_document_get_node(&doc, p->key);
                if (key_node.type == YAML_SCALAR_NODE)
                {
                    std::string key(resolve_variables(reinterpret_cast<const char*>(key_node.data.scalar.value)));
                    if (key == "variables")
                    {
                        extract_variables(doc, *yaml_document_get_node(&doc, p->value));
                    }
                    else
                    {
                        auto found = get_factories().find(key);
                        if (found == get_factories().end())
                        {
                            yaml_node_t* val = yaml_document_get_node(&doc, p->value);
                            if (mnto)
                            {
                                // If we're in a configurable and the map key is unknown,
                                // then the only nodes we can deal with are scalar mapped
                                // values.
                                if (val->type == YAML_SCALAR_NODE)
                                    handle(doc, *val, level + 1, key, mnto);
                                else
                                    report_error("Unknown YAML mapping: " + key);
                            }
                            else
                            {
                                // If there is no memento, then this is an unknown, which
                                // we'll handle properly in the scalar block.
                                handle(doc, *val, level + 1, key, mnto);
                            }
                        }
                        else
                        {
                            auto sub = found->second->create_memento(*this);
                            // pass empty key here because we've just consumed the key
                            handle(doc, *yaml_document_get_node(&doc, p->value), level + 1, "", sub);
                            auto cnf = found->second->create_configurable(sub);
                            if (mnto)
                                mnto->handle(std::move(cnf));
                        }
                    }
                }
            }
        }
        else if (node.type == YAML_SEQUENCE_NODE)
        {
            for (yaml_node_item_t* i = node.data.sequence.items.start;
                 i < node.data.sequence.items.top;
                 i++)
            {
                handle(doc, *yaml_document_get_node(&doc, *i), level + 1, key, mnto);
            }
        }
    }
    catch (std::exception& e)
    {
        report_error(std::string("An error occurred processing the YAML: ") + e.what());
    }
}

}
