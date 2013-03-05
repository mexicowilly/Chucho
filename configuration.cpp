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

chucho::configuration::style sty = chucho::configuration::style::AUTOMATIC;
std::string file_name(std::string(1, '.') + chucho::file::dir_sep + std::string("chucho.yaml"));
bool allow_default_config = true;
bool allow_environment_var = true;
std::string fallback;

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

namespace configuration
{

bool allow_default()
{
    return allow_default_config;
}

bool allow_environment_variable()
{
    return allow_environment_var;
}

const std::string& get_fallback()
{
    return fallback;
}

const std::string& get_file_name()
{
    return file_name;
}

style get_style()
{
    return sty;
}

void perform()
{
    reporter report;

    if (sty == style::OFF)
    {
        report.warning("Configuration will not be performed because it has been turned off");
        return;
    }
    configurator::initialize();
    bool got_config = false;
    std::string fn;
    if (allow_environment_var)
    {
        char* ev = std::getenv("CHUCHO_CONFIG");
        if (ev != nullptr)
            fn = ev;
    }
    if (fn.empty())
        fn = file_name;
    yaml_configurator yam;
    if (file::exists(fn))
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
    if (!got_config && !fallback.empty())
    {
        try
        {
            // this is already validated UTF-8
            std::istringstream fb_in(fallback);
            yam.configure(fb_in);
            got_config = true;
            report.info("Using the fallback configuration");
        }
        catch (std::exception& e)
        {
            report.error("Error setting fallback configuration: " + exception::nested_whats(e));
        }
    }
    if (!got_config && allow_default_config)
    {
        set_default_config();
        report.info("Using the default configuration");
    }
}

void set_allow_default(bool allow)
{
    allow_default_config = allow;
}

void set_allow_environment_variable(bool allow)
{
    allow_environment_var = allow;
}

void set_fallback(const std::string& config)
{
    std::istringstream stream(config);
    utf8::validate(stream);
    fallback = config;
}

void set_file_name(const std::string& name)
{
    file_name = name;
}

void set_style(style stl)
{
    sty = stl;
}

}

}
