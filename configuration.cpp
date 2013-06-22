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
#include <cstdlib>
#include <fstream>
#include <assert.h>

namespace chucho
{

extern std::shared_ptr<chucho::logger> root_logger;

}

namespace
{

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

void set_default_config()
{
    if (chucho::root_logger->get_writers().empty())
    {
        std::shared_ptr<chucho::formatter> fmt(
            new chucho::pattern_formatter("%d{%H:%M:%S.%q} %-5p %.36c - %m%n"));
        std::shared_ptr<chucho::writer> wrt(new chucho::cout_writer(fmt));
        chucho::root_logger->add_writer(wrt);
    }
}

}

namespace chucho
{

configuration::style configuration::style_ = style::AUTOMATIC;
std::string configuration::file_name_(std::string(1, '.') + chucho::file::dir_sep + std::string("chucho.yaml"));
bool configuration::allow_default_config_ = true;
std::string configuration::fallback_;
std::string configuration::environment_variable_("CHUCHO_CONFIG");
configuration::unknown_handler_type configuration::unknown_handler_;
std::string configuration::loaded_file_name_;
extern std::atomic<bool> is_initialized;

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
            loaded_file_name_ = file_name;
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

void configuration::perform()
{
    reporter report;

    if (style_ == style::OFF)
    {
        report.info("Configuration will not be performed because it has been turned off");
        return;
    }
    configurator::initialize();
    bool got_config = false;
    std::string fn;
    if (!environment_variable_.empty())
    {
        char* ev = std::getenv(environment_variable_.c_str());
        if (ev != nullptr)
            fn = ev;
    }
    if (fn.empty())
        fn = file_name_;
    if (!fn.empty())
    {
        if (file::exists(fn))
        {
            got_config = configure_from_yaml_file(fn, report);
            if (!got_config)
                logger::remove_unused_loggers();
        }
        else
        {
            report.warning("The file " + fn + " does not exist");
        }
    }
    if (!got_config && !fallback_.empty())
    {
        try
        {
            yaml_configurator yam;
            // this is already validated UTF-8
            std::istringstream fb_in(fallback_);
            yam.configure(fb_in);
            got_config = true;
            report.info("Using the fallback configuration");
        }
        catch (std::exception& e)
        {
            logger::remove_unused_loggers();
            report.error("Error setting fallback configuration: " + exception::nested_whats(e));
        }
    }
    if (!got_config && allow_default_config_)
    {
        set_default_config();
        report.info("Using the default configuration");
    }
}

bool configuration::reconfigure()
{
    bool result = false;
    if (is_initialized &&
        style_ == style::AUTOMATIC &&
        (!loaded_file_name_.empty() || !file_name_.empty()))
    {
        reporter report;
        std::string to_try = loaded_file_name_.empty() ?
            file_name_ : loaded_file_name_;
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

void configuration::set_fallback(const std::string& config)
{
    std::istringstream stream(config);
    utf8::validate(stream);
    fallback_ = config;
}

}
