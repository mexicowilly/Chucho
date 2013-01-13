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

void set_default_config()
{
    if (allow_default_config)
    {
        std::shared_ptr<chucho::logger> root = chucho::logger::get("");
        if (root->get_writers().empty())
        {
            std::shared_ptr<chucho::formatter> fmt(
                new chucho::pattern_formatter("%d{%H:%M:%S.%q} %-5p %.36c - %m%n"));
            std::shared_ptr<chucho::writer> wrt(new chucho::cout_writer(fmt));
            root->add_writer(wrt);
        }
    }
}

}

namespace chucho
{

extern bool automatically_configuring;

namespace configuration
{

bool allow_default()
{
    return allow_default_config;
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
    else if (sty == style::AUTOMATIC)
    {
        if (!automatically_configuring)
        {
            report.warning("Configuration will not be performed because it has been set to automatic");
            return;
        }
    }
    configurator::initialize();
    std::string fn;
    char* ev = std::getenv("CHUCHO_CONFIG");
    if (ev != nullptr)
        fn = ev;
    else
        fn = file_name;
    if (file::exists(fn))
    {
        std::ifstream in(fn.c_str());
        if (in.is_open())
        {
            try
            {
                utf8::validate(in);
                in.close();
                in.open(fn.c_str());
                yaml_configurator yam;
                yam.configure(in);
            }
            catch (exception& e)
            {
                report.error("Error reading " + fn +
                    ": " + exception::nested_whats(e));
            }
        }
        else
        {
            report.warning(fn + " exists, but I can't open it for reading");
            set_default_config();
        }
    }
    else
    {
        report.warning("The file " + fn + " does not exist");
        set_default_config();
    }
}

void set_allow_default(bool allow)
{
    allow_default_config = allow;
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
