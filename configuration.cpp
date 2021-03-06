/*
 * Copyright 2013-2021 Will Mason
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

#include <chucho/configuration.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <chucho/environment.hpp>
#include <chucho/configurator.hpp>
#include <chucho/configurable_factory.hpp>

#include <chucho/yaml_parser.hpp>
#include <chucho/yaml_configurator.hpp>
#include <chucho/config_file_configurator.hpp>
#include <chucho/properties.hpp>
#include <chucho/json_configurator.hpp>

#include <fstream>
#include <algorithm>
#include <sstream>
#include <assert.h>
#include <cstring>

namespace
{

struct static_data
{
    static_data();

    chucho::configuration::style style_;
    std::string file_name_;
    bool allow_default_config_;
    std::string fallback_;
    std::string environment_variable_;
    chucho::configuration::unknown_handler_type unknown_handler_;
    std::string loaded_file_name_;
    bool is_configured_;
    std::size_t max_size_;
    chucho::security_policy security_policy_;
};

static_data::static_data()
    : style_(chucho::configuration::style::AUTOMATIC),
      file_name_(std::string(1, '.') + chucho::file::dir_sep + std::string("chucho.yaml")),
      allow_default_config_(true),
      environment_variable_("CHUCHO_CONFIG"),
      is_configured_(false),
      max_size_(100 * 1024)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

static_data& data()
{
    static std::once_flag once;
    // This gets cleaned up in finalize()
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

void set_default_config(std::shared_ptr<chucho::logger> root_logger)
{
    if (root_logger->get_writer_names().empty())
    {
        auto fmt = std::make_unique<chucho::pattern_formatter>("%d{%H:%M:%S.%q} %-5p %.36c - %m%n");
        auto wrt = std::make_unique<chucho::cout_writer>("chucho::cout_writer", std::move(fmt));
        root_logger->add_writer(std::move(wrt));
    }
}

enum class format
{
    YAML,
    CONFIG_FILE,
    JSON,
    DONT_KNOW
};

format detect_text_format(const std::string& text)
{
    std::istringstream stream1(text);
    chucho::yaml_parser prs(stream1);
    yaml_document_t doc;
    bool good = false;
    if (yaml_parser_load(prs, &doc))
    {
        auto root = yaml_document_get_root_node(&doc);
        yaml_node_type_t tp = root->type;
        if (tp == YAML_MAPPING_NODE)
        {
            for (auto mn = root->data.mapping.pairs.start;
                 mn < root->data.mapping.pairs.top;
                 mn++)
            {
                auto k = reinterpret_cast<char*>(yaml_document_get_node(&doc, mn->key)->data.scalar.value);
                if (std::strcmp(k, "chucho::logger") == 0)
                {
                    good = true;
                    break;
                }
            }
        }
        else if (tp == YAML_SEQUENCE_NODE)
        {
            for (auto item = root->data.sequence.items.start;
                 item < root->data.sequence.items.top;
                 item++)
            {
                auto sn = yaml_document_get_node(&doc, *item);
                if (sn->type == YAML_MAPPING_NODE)
                {
                    auto k = reinterpret_cast<char*>(yaml_document_get_node(&doc, sn->data.mapping.pairs.start->key)->data.scalar.value);
                    if (std::strcmp(k, "chucho::logger") == 0)
                    {
                        good = true;
                        break;
                    }
                }
            }
        }
        yaml_document_delete(&doc);
        if (good)
            return format::YAML;
    }
    auto json = cJSON_Parse(text.c_str());
    if (json != nullptr)
    {
        if (cJSON_GetObjectItem(json, "chucho_loggers") != nullptr)
            good = true;
        cJSON_Delete(json);
        if (good)
            return format::JSON;
    }
    std::istringstream stream2(text);
    chucho::properties props(stream2);
    if (!props.empty())
        return format::CONFIG_FILE;

    return format::DONT_KNOW;
}

format detect_file_format(const std::string& file_name)
{
    std::ifstream stream(file_name);
    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    return detect_text_format(str);
}

}

namespace chucho
{

bool configuration::allow_default()
{
    return data().allow_default_config_;
}

bool configuration::configure_from_file(const std::string& file_name, reporter& report)
{
    bool result = false;
    format fmt = detect_file_format(file_name);
    std::unique_ptr<configurator> cfg;

    if (fmt == format::YAML)
    {
        report.info("The file, " + file_name + ", is in YAML format");
        cfg = std::make_unique<yaml_configurator>(data().security_policy_);
    }
    else if (fmt == format::JSON)
    {
        report.info("The file, " + file_name + ", is in JSON format");
        cfg = std::make_unique<json_configurator>(data().security_policy_);
    }
    else if (fmt == format::CONFIG_FILE)
    {
        report.info("The file, " + file_name + ", is in config file format");
        cfg = std::make_unique<config_file_configurator>(data().security_policy_);
    }
    else if (fmt == format::DONT_KNOW)
    {
        report.warning("Unable to determine the format of the file " + file_name);
        return result;
    }

    std::ifstream in(file_name.c_str());
    if (in.is_open())
    {
        try
        {
            cfg->configure(in);
            data().loaded_file_name_ = file_name;
            result = true;
        }
        catch (std::exception& e)
        {
            report.error("Error reading " + file_name +
                ": " + chucho::exception::nested_whats(e));
        }
    }
    else
    {
        report.warning(file_name + " exists, but I can't open it for reading");
    }
    return result;
}

bool configuration::configure_from_text(const std::string& cfg, reporter& report)
{
    bool result = true;
    static_data& sd(data());
    try
    {
        format fmt = detect_text_format(cfg);

        if (fmt == format::YAML)
        {
            yaml_configurator yam(sd.security_policy_);
            std::istringstream in(cfg);
            yam.configure(in);
            report.info("Using the YAML format configuration"); 
        }
        else if (fmt == format::JSON)
        {
            json_configurator js(sd.security_policy_);
            std::istringstream in(cfg);
            js.configure(in);
            report.info("Using the JSON format configuration");
        }
        else if (fmt == format::CONFIG_FILE)
        {
            config_file_configurator cnf(sd.security_policy_);
            std::istringstream in(cfg);
            cnf.configure(in);
            report.info("Using the config file format configuration"); 
        }
        else if (fmt == format::DONT_KNOW)
        {
            report.warning("Unable to detect the format of the configuration");
            result = false;
        }
    }
    catch (std::exception& e)
    {
        logger::remove_unused_loggers();
        report.error("Error setting configuration: " + exception::nested_whats(e));
        result = false;
    }
    return result;
}

const std::string& configuration::get_environment_variable()
{
    return data().environment_variable_;
}

const std::string& configuration::get_fallback()
{
    return data().fallback_;
}

const std::string& configuration::get_file_name()
{
    return data().file_name_;
}

const std::string& configuration::get_loaded_file_name()
{
    return data().loaded_file_name_;
}

std::size_t configuration::get_max_size()
{
    return data().max_size_;
}

security_policy& configuration::get_security_policy()
{
    static std::once_flag once;

    std::call_once(once, initialize_security_policy);
    return data().security_policy_;
}

configuration::style configuration::get_style()
{
    return data().style_;
}

configuration::unknown_handler_type configuration::get_unknown_handler()
{
    return data().unknown_handler_;
}

void configuration::initialize_security_policy()
{
    // We only need one of the formats
    yaml_configurator cnf(data().security_policy_);
    // The mementos are where each configurable configures
    // its security policy.
    auto& facts(configurator::get_factories());
    for (const auto& fact : facts)
        fact.second->create_memento(cnf);
}

void configuration::perform(std::shared_ptr<logger> root_logger)
{
    static_data& sd(data());
    reporter report;

    if (sd.style_ == style::OFF)
    {
        report.info("Configuration will not be performed because it has been turned off");
        return;
    }

    configurator::initialize();
    std::string fn;
    if (!sd.environment_variable_.empty())
    {
        auto ev = environment::get(sd.environment_variable_);
        if (ev)
            fn = *ev;
    }
    if (fn.empty())
        fn = sd.file_name_;
    if (!fn.empty())
    {
        if (file::exists(fn))
        {
            auto sz = file::size(fn);
            if (sz > sd.max_size_)
            {
                report.warning("The file, " + fn + ", is " + std::to_string(sz) +
                    " bytes large, but the maximum allowed is " + std::to_string(sd.max_size_));
            }
            else
            {
                sd.is_configured_ = configure_from_file(fn, report);
                if (!sd.is_configured_) 
                    logger::remove_unused_loggers();
            }
        }
        else
        {
            report.warning("The file " + fn + " does not exist");
        }
    }
    if (!sd.is_configured_ && !sd.fallback_.empty())
    {
        sd.is_configured_ = configure_from_text(sd.fallback_, report);
        if (!sd.is_configured_)
            logger::remove_unused_loggers();
    }

    if (!sd.is_configured_ && sd.allow_default_config_)
    {
        set_default_config(root_logger);
        report.info("Using the default configuration");
        sd.is_configured_ = true;
    }
}

bool configuration::reconfigure()
{
    static_data& sd(data());
    bool result = false;
    if (sd.is_configured_ &&
        sd.style_ == style::AUTOMATIC &&
        (!sd.loaded_file_name_.empty() || !sd.file_name_.empty()))
    {
        reporter report;
        std::string to_try = sd.loaded_file_name_.empty() ?
            sd.file_name_ : sd.loaded_file_name_;
        if (file::exists(to_try))
        {
            auto sz = file::size(to_try);
            if (sz > sd.max_size_)
            {
                report.warning("The file, " + to_try + ", is " + std::to_string(sz) +
                    " bytes large, but the maximum allowed is " + std::to_string(sd.max_size_));
            }
            else
            {
                auto loggers = logger::get_existing_loggers();
                for (auto lgr : loggers)
                    lgr->reset();
                if (configure_from_file(to_try, report))
                    result = true;
            }
        }
        else
        {
            report.warning("The file " + to_try + " does not exist");
        }
    }
    return result;
}

void configuration::set_allow_default(bool allow)
{
    data().allow_default_config_ = allow;
}

bool configuration::set(const std::string& cfg)
{
    reporter report;
    bool result = false;
    if (cfg.length() > data().max_size_)
    {
        report.error("The configuration size is " + std::to_string(cfg.length()) +
            ", but the maximum allowed is " + std::to_string(data().max_size_));
    }
    else
    {
        auto loggers = logger::get_existing_loggers();
        for (auto lgr : loggers)
            lgr->reset();
        configurator::initialize();
        if (configure_from_text(cfg, report))
            result = true;
    }
    return result;
}

void configuration::set_environment_variable(const std::string& var)
{
    data().environment_variable_ = var;
}

void configuration::set_fallback(const std::string& config)
{
    if (config.length() > data().max_size_) 
    {
        reporter report;
        report.error("The fallback size is " + std::to_string(config.length()) +
            ", but the maximum allowed is " + std::to_string(data().max_size_));
    }
    else
    {
        data().fallback_ = config;
    }
}

void configuration::set_file_name(const std::string& name)
{
    data().file_name_ = name;
}

void configuration::set_max_size(std::size_t mx)
{
    data().max_size_ = mx;
}

void configuration::set_style(style stl)
{
    data().style_ = stl;
}

void configuration::set_unknown_handler(unknown_handler_type hndl)
{
    data().unknown_handler_ = hndl;
}

}
