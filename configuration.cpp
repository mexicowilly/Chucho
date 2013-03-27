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
#include <chucho/status_reporter.hpp>
#include <chucho/yaml_configurator.hpp>
#include <chucho/utf8_validator.hpp>
#include <chucho/exception.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/logger.hpp>
#include <cstdlib>
#include <fstream>

namespace chucho
{

extern std::shared_ptr<chucho::logger> root_logger;

}

namespace
{

class reporter : public chucho::status_reporter
{
public:
    reporter()
    {
        set_status_origin("configuration");
    }

    void error(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const
    {
        report_error(message, ex);
    }

    void info(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const
    {
        report_info(message, ex);
    }

    void warning(const std::string& message, std::exception_ptr ex = std::exception_ptr()) const
    {
        report_warning(message, ex);
    }
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

void configuration::perform()
{
    reporter report;

    if (style_ == style::OFF)
    {
        report.warning("Configuration will not be performed because it has been turned off");
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
    yaml_configurator yam;
    if (!fn.empty() && file::exists(fn))
    {
        std::ifstream val_in(fn.c_str());
        if (val_in.is_open())
        {
            try
            {
                utf8::validate(val_in);
                val_in.close();
                std::ifstream yam_in(fn.c_str());
                yam.configure(yam_in);
                got_config = true;
            }
            catch (std::exception& e)
            {
                report.error("Error reading " + fn +
                    ": " + exception::nested_whats(e));
            }
        }
        else
        {
            report.warning(fn + " exists, but I can't open it for reading");
        }
    }
    else
    {
        report.warning("The file " + fn + " does not exist");
    }
    if (!got_config && !fallback_.empty())
    {
        try
        {
            // this is already validated UTF-8
            std::istringstream fb_in(fallback_);
            yam.configure(fb_in);
            got_config = true;
            report.info("Using the fallback configuration");
        }
        catch (std::exception& e)
        {
            report.error("Error setting fallback configuration: " + exception::nested_whats(e));
        }
    }
    if (!got_config && allow_default_config_)
    {
        set_default_config();
        report.info("Using the default configuration");
    }
}

void configuration::set_fallback(const std::string& config)
{
    std::istringstream stream(config);
    utf8::validate(stream);
    fallback_ = config;
}

}
