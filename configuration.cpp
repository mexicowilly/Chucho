/*
 * Copyright 2013-2017 Will Mason
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

#if defined(CHUCHO_YAML_CONFIG)
#include <chucho/yaml_parser.hpp>
#include <chucho/yaml_configurator.hpp>
#endif
#if defined(CHUCHO_CONFIG_FILE_CONFIG)
#include <chucho/config_file_configurator.hpp>
#include <chucho/properties.hpp>
#endif
#if defined(CHUCHO_JSON_CONFIG)
#include <cJSON.h>
#endif

#include <fstream>
#include <algorithm>
#include <sstream>
#include <assert.h>

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

// Not to be confused with the logger_memento class that can only be
// used during configuration. This struct is like a GoF memento in
// that is stores state for later reuse.
class logger_state
{
public:
    logger_state(std::shared_ptr<chucho::logger> lgr)
        : name_(lgr->get_name()),
          level_(lgr->get_level()),
          writers_(lgr->get_writers()),
          writes_to_ancestors_(lgr->writes_to_ancestors())
    {
    }

    void restore(std::shared_ptr<chucho::logger> lgr)
    {
        assert(lgr->get_name() == name_);
        lgr->reset();
        lgr->set_level(level_);
        std::for_each(writers_.begin(),
                      writers_.end(),
                      [&] (std::shared_ptr<chucho::writer> wrt) { lgr->add_writer(wrt); });
        lgr->set_writes_to_ancestors(writes_to_ancestors_);

    }

private:
    std::string name_;
    std::shared_ptr<chucho::level> level_;
    std::vector<std::shared_ptr<chucho::writer>> writers_;
    bool writes_to_ancestors_;
};

void set_default_config(std::shared_ptr<chucho::logger> root_logger)
{
    if (root_logger->get_writers().empty())
    {
        std::shared_ptr<chucho::formatter> fmt(
            new chucho::pattern_formatter("%d{%H:%M:%S.%q} %-5p %.36c - %m%n"));
        std::shared_ptr<chucho::writer> wrt(new chucho::cout_writer(fmt));
        root_logger->add_writer(wrt);
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
#if defined(CHUCHO_YAML_CONFIG)

    std::istringstream stream1(text);
    chucho::yaml_parser prs(stream1);
    yaml_document_t doc;
    if (yaml_parser_load(prs, &doc))
    {
        yaml_node_type_t tp = yaml_document_get_root_node(&doc)->type;
        yaml_document_delete(&doc);
        // A config file format will have one scalar node. A valid
        // Chucho YAML config will not have any scalar nodes at the
        // top level.
        if (tp != YAML_SCALAR_NODE)
            return format::YAML;
    }

#endif

#if defined(CHUCHO_JSON_CONFIG)

    auto json = cJSON_Parse(text.c_str());
    if (json != nullptr)
    {
        cJSON_Delete(json);
        return format::JSON;
    }

#endif

#if defined(CHUCHO_CONFIG_FILE_CONFIG)

    std::istringstream stream2(text);
    chucho::properties props(stream2);
    if (props.size() > 0)
        return format::CONFIG_FILE;

#endif

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

    #if defined(CHUCHO_YAML_CONFIG)

    if (fmt == format::YAML)
    {
        report.info("The file, " + file_name + ", is in YAML format");
        cfg.reset(new yaml_configurator(data().security_policy_));
    }

    #endif

    #if defined(CHUCHO_CONFIG_FILE_CONFIG)

    if (fmt == format::CONFIG_FILE)
    {
        report.info("The file, " + file_name + ", is in config file format");
        cfg.reset(new config_file_configurator(data().security_policy_));
    }

    #endif

    if (fmt == format::DONT_KNOW)
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
    bool result = false;
    static_data& sd(data());
    try
    {
        format fmt = detect_text_format(cfg);

        #if defined(CHUCHO_YAML_CONFIG)

        if (fmt == format::YAML)
        {
            yaml_configurator yam(sd.security_policy_);
            std::istringstream in(cfg);
            yam.configure(in);
            report.info("Using the YAML format configuration"); 
        }

        #endif

        #if defined(CHUCHO_CONFIG_FILE_CONFIG)

        if (fmt == format::CONFIG_FILE)
        {
            config_file_configurator cnf(sd.security_policy_);
            std::istringstream in(cfg);
            cnf.configure(in);
            report.info("Using the config file format configuration"); 
        }

        #endif

        if (fmt == format::DONT_KNOW)
            report.warning("Unable to detect the format of the configuration");
        else
            result = true;
    }
    catch (std::exception& e)
    {
        logger::remove_unused_loggers();
        report.error("Error setting configuration: " + exception::nested_whats(e));
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
    #if defined(CHUCHO_YAML_CONFIG) || defined(CHUCHO_CONFIG_FILE_CONFIG)

    #if defined(CHUCHO_YAML_CONFIG)

    yaml_configurator cnf(data().security_policy_);

    #elif defined(CHUCHO_CONFIG_FILE_CONFIG)

    config_file_configurator cnf(data().security_policy_);

    #else

    return;

    #endif
    // The mementos are where each configurable configures
    // its security policy.
    auto& facts(configurator::get_factories());
    for (auto fact : facts)
        fact.second->create_memento(cnf);

    #endif
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

    #if defined(CHUCHO_YAML_CONFIG) || defined(CHUCHO_CONFIG_FILE_CONFIG)

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

    #endif

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
                std::vector<logger_state> states;
                for (auto lgr : loggers)
                {
                    states.emplace_back(lgr);
                    lgr->reset();
                }
                if (configure_from_file(to_try, report))
                {
                    result = true;
                }
                else
                {
                    for (unsigned i = 0; i < loggers.size(); i++)
                        states[i].restore(loggers[i]);
                }
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
        std::vector<logger_state> states;
        for (auto lgr : loggers)
        {
            states.emplace_back(lgr);
            lgr->reset();
        }
        configurator::initialize();
        if (configure_from_text(cfg, report))
        {
            result = true;
        }
        else
        {
            for (unsigned i = 0; i < loggers.size(); i++)
                states[i].restore(loggers[i]);
        }
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
