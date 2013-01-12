#include <chucho/configurator.hpp>
#include <chucho/exception.hpp>
#include <chucho/cerr_writer_factory.hpp>
#include <chucho/cout_writer_factory.hpp>
#include <chucho/file_writer_factory.hpp>
#include <chucho/level_threshold_filter_factory.hpp>
#include <chucho/logger_factory.hpp>
#include <chucho/numbered_file_roller_factory.hpp>
#include <chucho/pattern_formatter_factory.hpp>
#include <chucho/rolling_file_writer_factory.hpp>
#include <chucho/size_file_roll_trigger_factory.hpp>
#include <chucho/time_file_roller_factory.hpp>
#include <regex>

namespace chucho
{

configurator::configurator()
{
    set_status_origin("configurator");
}

std::map<std::string, std::shared_ptr<configurable_factory>>& configurator::get_factories()
{
    static std::map<std::string, std::shared_ptr<configurable_factory>> factories;

    return factories;
}

void configurator::initialize()
{
    std::shared_ptr<configurable_factory> fact(new cerr_writer_factory());
    add_configurable_factory("chucho::cerr_writer", fact);
    fact.reset(new cout_writer_factory());
    add_configurable_factory("chucho::cout_writer", fact);
    fact.reset(new file_writer_factory());
    add_configurable_factory("chucho::file_writer", fact);
    fact.reset(new level_threshold_filter_factory());
    add_configurable_factory("chucho::level_threshold_filter", fact);
    fact.reset(new logger_factory());
    add_configurable_factory("chucho::logger", fact);
    fact.reset(new numbered_file_roller_factory());
    add_configurable_factory("chucho::numbered_file_roller", fact);
    fact.reset(new pattern_formatter_factory());
    add_configurable_factory("chucho::pattern_formatter", fact);
    fact.reset(new rolling_file_writer_factory());
    add_configurable_factory("chucho::rolling_file_writer", fact);
    fact.reset(new size_file_roll_trigger_factory());
    add_configurable_factory("chucho::size_file_roll_trigger", fact);
    fact.reset(new time_file_roller_factory());
    add_configurable_factory("chucho::time_file_roller", fact);
}

std::string configurator::resolve_variables(const std::string& val)
{
    static std::regex re("\\$\\{(.+?)\\}");

    std::string result(val);
    int pos_offset = 0;
    auto itor = std::sregex_iterator(val.begin(), val.end(), re);
    auto end = std::sregex_iterator();
    while (itor != end)
    {
        const std::smatch& sm(*itor);
        auto found = variables_.find(sm[1]);
        if (found != variables_.end())
        {
            result.replace(sm.position() + pos_offset, sm.length(), found->second);
            pos_offset += found->second.length() - sm.length();
        }
        ++itor;
    }
    return result;
}

}
