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

#include <chucho/configuration.hpp>
#include <chucho/file.hpp>
#include <chucho/yaml_configurator.hpp>
#include <chucho/utf8.hpp>
#include <chucho/exception.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <cstdlib>
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
    bool is_configured;
};

static_data::static_data()
    : style_(chucho::configuration::style::AUTOMATIC),
      file_name_(std::string(1, '.') + chucho::file::dir_sep + std::string("chucho.yaml")),
      allow_default_config_(true),
      environment_variable_("CHUCHO_CONFIG"),
      is_configured(false)
{
    chucho::garbage_cleaner::get().add([this] () { delete this; });
}

std::once_flag once;

static_data& data()
{
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

}

namespace chucho
{

bool configuration::allow_default()
{
    return data().allow_default_config_;
}

bool configuration::configure_from_yaml_file(const std::string& file_name, reporter& report)
{
    std::ifstream val_in(file_name.c_str());
    if (val_in.is_open())
    {
        try
        {
            chucho::utf8::validate(val_in);
            val_in.close();
            std::ifstream yam_in(file_name.c_str());
            chucho::yaml_configurator yam;
            yam.configure(yam_in);
            data().loaded_file_name_ = file_name;
            return true;
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
    return false;
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

configuration::style configuration::get_style()
{
    return data().style_;
}

configuration::unknown_handler_type configuration::get_unknown_handler()
{
    return data().unknown_handler_;
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
        char* ev = std::getenv(sd.environment_variable_.c_str());
        if (ev != nullptr)
            fn = ev;
    }
    if (fn.empty())
        fn = sd.file_name_;
    if (!fn.empty())
    {
        if (file::exists(fn))
        {
            sd.is_configured = configure_from_yaml_file(fn, report);
            if (!sd.is_configured)
                logger::remove_unused_loggers();
        }
        else
        {
            report.warning("The file " + fn + " does not exist");
        }
    }
    if (!sd.is_configured && !sd.fallback_.empty())
    {
        try
        {
            yaml_configurator yam;
            // this is already validated UTF-8
            std::istringstream fb_in(sd.fallback_);
            yam.configure(fb_in);
            sd.is_configured = true;
            report.info("Using the fallback configuration");
        }
        catch (std::exception& e)
        {
            logger::remove_unused_loggers();
            report.error("Error setting fallback configuration: " + exception::nested_whats(e));
        }
    }
    if (!sd.is_configured && sd.allow_default_config_)
    {
        set_default_config(root_logger);
        report.info("Using the default configuration");
        sd.is_configured = true;
    }
}

bool configuration::reconfigure()
{
    static_data& sd(data());
    bool result = false;
    if (sd.is_configured &&
        sd.style_ == style::AUTOMATIC &&
        (!sd.loaded_file_name_.empty() || !sd.file_name_.empty()))
    {
        reporter report;
        std::string to_try = sd.loaded_file_name_.empty() ?
            sd.file_name_ : sd.loaded_file_name_;
        if (file::exists(to_try))
        {
            auto loggers = logger::get_existing_loggers();
            std::vector<logger_state> states;
            for (auto lgr : loggers)
            {
                states.emplace_back(lgr);
                lgr->reset();
            }
            if (configure_from_yaml_file(to_try, report))
            {
                result = true;
            }
            else
            {
                for (unsigned i = 0; i < loggers.size(); i++)
                    states[i].restore(loggers[i]);
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

void configuration::set_environment_variable(const std::string& var)
{
    data().environment_variable_ = var;
}

void configuration::set_fallback(const std::string& config)
{
    std::istringstream stream(config);
    utf8::validate(stream);
    data().fallback_ = config;
}

void configuration::set_file_name(const std::string& name)
{
    data().file_name_ = name;
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
