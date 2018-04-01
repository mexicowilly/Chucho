/*
 * Copyright 2013-2018 Will Mason
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

#include <chucho/config_file_configurator.hpp>
#include <chucho/properties.hpp>
#include <chucho/configurable_factory.hpp>
#include <chucho/exception.hpp>
#include <chucho/regex.hpp>
#include <chucho/pattern_formatter.hpp>
#include <chucho/cout_writer.hpp>
#include <chucho/cerr_writer.hpp>
#include <chucho/text_util.hpp>
#include <chucho/file_writer_memento.hpp>
#include <chucho/level_threshold_filter.hpp>
#include <chucho/move_util.hpp>
#include <algorithm>
#include <assert.h>

namespace
{

class writer_creation_exception : public chucho::exception
{
public:
    writer_creation_exception(const std::string& name);
};

writer_creation_exception::writer_creation_exception(const std::string& name)
    : exception("Could not create the writer named " + name)
{
}

chucho::properties get_non_empty_subset(const chucho::properties& props,
                                        const std::string& prefix)
{
    chucho::properties sub = props.get_subset(prefix);
    if (sub.empty()) 
        throw chucho::exception("No keys with prefix " + prefix + " were found");
    return sub;
}

}

namespace chucho
{

config_file_configurator::config_file_configurator(security_policy& sec_pol)
    : configurator(sec_pol),
      memento_key_set_(memento_key_set::CHUCHO)
{
    set_status_origin("config_file_configurator");
}

void config_file_configurator::configure(std::istream& in)
{
    std::unique_ptr<properties_processor> proc;
    properties props(in);

    #if defined(CHUCHO_CONFIG_FILE)

    std::unique_ptr<properties> id(new properties(props.get_subset("chucho.")));
    if (!id->empty())
    {
        proc.reset(new chucho_properties_processor(*this));
    }
    else
    {
        #if defined(CHUCHO_LOG4CPLUS_FILE)

        id.reset(new properties(props.get_subset("log4cplus.")));
        if (id->empty())
            throw exception("Found neither Chucho nor log4cplus keys in the configuration");
        // Valgrind says this pointer is leaking, but it is confused
        proc.reset(new log4cplus_properties_processor(*this));
        memento_key_set_ = memento_key_set::LOG4CPLUS;

        #else

        throw exception("Found no Chucho keys in the configuration");

        #endif
    }

    #elif defined(CHUCHO_LOG4CPLUS_FILE)

    std::unique_ptr<properties> id(new properties(props.get_subset("log4cplus.")));
    if (id->empty())
        throw exception("Found no log4cplus keys in the configuration");
    proc.reset(new log4cplus_properties_processor(*this));
    memento_key_set_ = memento_key_set::LOG4CPLUS;

    #endif

    id.reset();
    proc->process(props);
}

config_file_configurator::properties_processor::properties_processor(config_file_configurator& cfg)
    : cfg_(cfg)
{
}

#if defined(CHUCHO_CONFIG_FILE)

config_file_configurator::chucho_properties_processor::chucho_properties_processor(config_file_configurator& cfg)
    : properties_processor(cfg)
{
    cfg_.report_info("Using Chucho-style config file configuration");
}

std::unique_ptr<configurable> config_file_configurator::chucho_properties_processor::create_configurable(const std::string& type,
                                                                                                         const std::string& name,
                                                                                                         const properties& props)
{
    auto fact = get_factory(type, name, props);
    auto mnto = std::move(fact->create_memento(cfg_));
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
        mnto->handle(cur.first, cur.second);
    return std::move(fact->create_configurable(mnto));
}

std::unique_ptr<configurable> config_file_configurator::chucho_properties_processor::create_file_roller(const std::string& name,
                                                                                                        const properties& props)
{
    std::string type("file_roller");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
    {
        if (cur.first == "file_compressor")
            mnto->handle(create_configurable(cur.first, cur.second, props));
        else 
            mnto->handle(cur.first, cur.second);
    }
    return fact->create_configurable(mnto); 
}

std::unique_ptr<configurable> config_file_configurator::chucho_properties_processor::create_writer(const std::string& name,
                                                                                                   const properties& props)
{
    std::string type("writer");
    auto fact = get_factory(type, name, props); 
    auto mnto = std::move(fact->create_memento(cfg_));
    auto cur_props = props.get_subset(type + '.' + name + '.');
    std::vector<std::string> generics;
    // VS2012 does not support initializer lists. Piece of shit.
    generics.push_back("filter");
    generics.push_back("formatter");
    generics.push_back("file_roll_trigger");
    generics.push_back("serializer");
    generics.push_back("compressor");
    #if defined(CHUCHO_HAVE_CURL)
    generics.push_back("email_trigger");
    #endif
    for (const auto& cur : cur_props)
    {
        if (cur.first == "file_roller")
            mnto->handle(std::move(create_file_roller(cur.second, props)));
        else if (cur.first == "writer")
            mnto->handle(std::move(create_writer(cur.second, props)));
        else if (std::count(generics.begin(), generics.end(), cur.first) > 0)
            mnto->handle(std::move(create_configurable(cur.first, cur.second, props)));
        else if (cur.first.find('.') == std::string::npos)
            mnto->handle(cur.first, cur.second); 
    }
    return std::move(fact->create_configurable(mnto));
}

std::shared_ptr<configurable_factory> config_file_configurator::chucho_properties_processor::get_factory(const std::string& type,
                                                                                                         const std::string& name,
                                                                                                         const properties& props)
{
    auto cls = props.get_one(type + '.' + name);
    if (!cls)
        throw exception("No class has been specified for " + type + " named " + name);
    auto fact = get_factories().find(*cls);
    if (fact == get_factories().end())
        throw exception("No " + type + " named " + *cls + " exists");
    return fact->second;
}

void config_file_configurator::chucho_properties_processor::process(const properties& props)
{
    auto chuprops = props.get_subset("chucho.");
    auto loggers = props.get("chucho.logger");
    assert(get_factories().find("chucho::logger") != get_factories().end());
    auto lgr_fact = get_factories().find("chucho::logger")->second;
    while (loggers.first != loggers.second)
    {
        try
        {
            auto mnto = lgr_fact->create_memento(cfg_);
            mnto->handle("name", loggers.first->second);
            properties cur_props = props.get_subset("chucho.logger." + loggers.first->second + '.');
            for (auto cur : cur_props)
            {
                if (cur.first == "writer")
                    mnto->handle(create_writer(cur.second, chuprops));
                else if (cur.first.find('.') == std::string::npos)
                    mnto->handle(cur.first, cur.second); 
            }
            lgr_fact->create_configurable(mnto);
        }
        catch (std::exception& e) 
        {
            cfg_.report_error("[logger " + loggers.first->second +
                "] An error occurred processing the config file: " + e.what());
        }
        ++loggers.first;
    }
}

#endif

#if defined(CHUCHO_LOG4CPLUS_FILE)

config_file_configurator::log4cplus_properties_processor::log4cplus_properties_processor(config_file_configurator& cfg)
    : properties_processor(cfg)
{
    cfg_.report_info("Using log4cplus-style config file configuration");
    factory_keys_["log4cplus::FileAppender"] = "chucho::file_writer";
    factory_keys_["log4cplus::SocketAppender"] = "chucho::remote_writer";
    factory_keys_["log4cplus::SysLogAppender"] = "chucho::syslog_writer";
    factory_keys_["log4cplus::spi::LogLevelMatchFilter"] = "chucho::level_filter";
    factory_keys_["log4cplus::spi::LogLevelRangeFilter"] = "chucho::level_threshold_filter";
    factory_keys_["log4cplus::NTEventLogAppender"] = "chucho::windows_event_log_writer";
}

void config_file_configurator::log4cplus_properties_processor::add_filters(writer& wrt,
                                                                           const properties& props)
{
    auto flts = props.get_subset("filters.");
    std::size_t num = 1;
    std::string name = std::to_string(num);
    optional<std::string> type = flts.get_one(name);
    while (type)
    {
        if (*type == "log4cplus::spi::DenyAllFilter")
        {
            auto flt = std::make_unique<level_threshold_filter>("level_threshold_filter", level::OFF_());
            wrt.add_filter(std::move(flt));
        }
        else if (*type == "log4cplus::spi::StringMatchFilter") 
        {
            cfg_.report_info("Chucho does not have an equivalent of StringMatchFilter");
        }
        else
        {
            auto key = factory_keys_.find(*type);
            if (key == factory_keys_.end())
            {
                cfg_.report_warning("Unknown log4cplus key: " + *type);
            }
            else
            {
                auto fact = cfg_.get_factories().find(key->second);
                assert(fact != cfg_.get_factories().end());
                auto mnto = std::move(fact->second->create_memento(cfg_));
                auto flt_props = get_non_empty_subset(flts, name + ".");
                for (auto prp : flt_props)
                {
                    if (prp.second.find(*type) != 0)
                        mnto->handle(prp.first, prp.second); 
                }
                auto filt = dynamic_move<filter>(fact->second->create_configurable(mnto));
                assert(filt);
                wrt.add_filter(std::move(filt));
            }
        }
        name = std::to_string(++num);
        type = flts.get_one(name);
    }
}

bool config_file_configurator::log4cplus_properties_processor::boolean_value(const std::string& text) const
{
    std::string low = text_util::to_lower(text);
    return low == "0" || low == "false" ? false : true;
}

std::unique_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_async_writer(const properties& top_props,
                                                                                                            const properties& wrt_props)
{
    assert(cfg_.get_factories().find("chucho::async_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::async_writer")->second;
    auto mnto = std::move(fact->create_memento(cfg_));
    auto app = wrt_props.get_one("Appender");
    if (app)
        mnto->handle(std::move(create_writer(*app, top_props)));
    return std::move(fact->create_configurable(mnto));
}

std::unique_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_console_writer(std::unique_ptr<formatter>&& fmt,
                                                                                                              const properties& props)
{
    auto type = props.get_one("logToStdErr");
    std::unique_ptr<configurable> result;
    if (type && boolean_value(*type))
        result = std::make_unique<cerr_writer>("chucho::cerr_writer", std::move(fmt));
    else
        result = std::make_unique<cout_writer>("chucho::cout_writer", std::move(fmt));
    return std::move(result);
}

std::unique_ptr<formatter> config_file_configurator::log4cplus_properties_processor::create_formatter(const std::string& type,
                                                                                                      const properties& props)
{
    std::unique_ptr<formatter> result;
    if (type == "log4cplus::PatternLayout")
    {
        auto pat = props.get_one("ConversionPattern");
        if (pat)
            result = std::make_unique<pattern_formatter>(*pat);
        else
            throw exception("A log4cplus::PatternLayout requires a ConversionPattern property");
    }
    else if (type == "log4cplus::SimpleLayout")
    {
        result = std::make_unique<pattern_formatter>("%p - %m%n");
    }
    else if (type == "log4cplus::TTCCLayout")
    {
        result = std::make_unique<pattern_formatter>("%r %t %p %c - %m%n");
    }
    else
    {
        throw exception("The layout type " + type + " is not recognized as a known log4cplus type");
    }
    return std::move(result);
}

void config_file_configurator::log4cplus_properties_processor::create_logger(const std::string& name,
                                                                             const std::string& desc,
                                                                             const properties& props)
{
    assert(get_factories().find("chucho::logger") != get_factories().end());
    auto lgr_fact = get_factories().find("chucho::logger")->second;
    auto mnto = std::move(lgr_fact->create_memento(cfg_));
    mnto->handle("name", name);
    auto tokens = split_logger_descriptor(desc);
    if (!tokens.empty())
        mnto->handle("level", tokens[0]);
    tokens.erase(tokens.begin());
    if (!tokens.empty())
    {
        for (auto tok : tokens)
        {
            auto wrt = std::move(create_writer(tok, props));
            if (wrt)
                mnto->handle(std::move(wrt));
        }
    }
    lgr_fact->create_configurable(mnto);
}

std::unique_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_size_rolling_writer(std::unique_ptr<formatter>&& fmt,
                                                                                                                   const properties& props)
{
    assert(cfg_.get_factories().find("chucho::rolling_file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::rolling_file_writer")->second;
    auto mnto = std::move(fact->create_memento(cfg_));
    fill_file_writer_memento(mnto, std::move(fmt), props);
    assert(cfg_.get_factories().find("chucho::numbered_file_roller") != cfg_.get_factories().end()); 
    auto roller_fact = cfg_.get_factories().find("chucho::numbered_file_roller")->second;
    auto roller_mnto = std::move(roller_fact->create_memento(cfg_));
    auto prop = props.get_one("MaxBackupIndex");
    roller_mnto->handle("max_index", (prop ? *prop : "1"));
    mnto->handle(std::move(roller_fact->create_configurable(roller_mnto)));
    assert(cfg_.get_factories().find("chucho::size_file_roll_trigger") != cfg_.get_factories().end()); 
    auto trigger_fact = cfg_.get_factories().find("chucho::size_file_roll_trigger")->second;
    auto trigger_mnto = std::move(trigger_fact->create_memento(cfg_));
    prop = props.get_one("MaxFileSize");
    trigger_mnto->handle("max_size", (prop ? *prop : "10m"));
    mnto->handle(std::move(trigger_fact->create_configurable(trigger_mnto)));
    return std::move(fact->create_configurable(mnto));
}

std::unique_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_time_rolling_writer(std::unique_ptr<formatter>&& fmt,
                                                                                                                   const properties& props)
{
    assert(cfg_.get_factories().find("chucho::rolling_file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::rolling_file_writer")->second;
    auto mnto = std::move(fact->create_memento(cfg_));
    fill_file_writer_memento(mnto, std::move(fmt), props);
    assert(cfg_.get_factories().find("chucho::time_file_roller") != cfg_.get_factories().end()); 
    auto roller_fact = cfg_.get_factories().find("chucho::time_file_roller")->second;
    auto roller_mnto = std::move(roller_fact->create_memento(cfg_));
    auto prop = props.get_one("MaxBackupIndex");
    roller_mnto->handle("max_history", prop ? *prop : "10");
    auto fwm = dynamic_cast<file_writer_memento*>(mnto.get());
    assert(fwm != nullptr);
    std::string fn = fwm->get_file_name();
    roller_mnto->handle("file_name_pattern", time_pattern_from_log4cplus_schedule(props.get_one("Schedule"), fn));
    mnto->handle(std::move(roller_fact->create_configurable(roller_mnto)));
    return std::move(fact->create_configurable(mnto));
}

std::unique_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_writer(const std::string& name,
                                                                                                      const properties& props)
{
    std::unique_ptr<configurable> result;
    auto type = props.get_one("appender." + name); 
    if (!type)
        throw exception("There is no definition of appender " + name);
    auto wrt_props = props.get_subset("appender." + name + "."); 
    auto fmt_type = wrt_props.get_one("layout"); 
    std::unique_ptr<formatter> fmt;
    if (fmt_type)
    {
        auto fmt_props = wrt_props.get_subset("layout.");
        fmt = std::move(create_formatter(*fmt_type, fmt_props));
    }
    if (*type == "log4cplus::ConsoleAppender") 
    {
        result = std::move(create_console_writer(std::move(fmt), wrt_props));
    }
    else if (*type == "log4cplus::RollingFileAppender")
    {
        result = std::move(create_size_rolling_writer(std::move(fmt), wrt_props));
    }
    else if (*type == "log4cplus::DailyRollingFileAppender")
    {
        result = std::move(create_time_rolling_writer(std::move(fmt), wrt_props));
    }
    else if (*type == "log4cplus::AsyncAppender")
    {
        result = std::move(create_async_writer(props, wrt_props));
    }
    else if (*type == "log4cplus::NullAppender")
    {
        return std::move(result);
    }
    else 
    {
        auto key = factory_keys_.find(*type);
        if (key == factory_keys_.end())
        {
            cfg_.report_warning("Unknown log4cplus key: " + *type);
        }
        else
        {
            auto fact = cfg_.get_factories().find(key->second);
            assert(fact != cfg_.get_factories().end());
            auto mnto = std::move(fact->second->create_memento(cfg_));
            if (fmt)
                mnto->handle(std::move(fmt));
            for (auto prp : wrt_props)
            {
                if (prp.first.find("layout") != 0 && prp.first.find("filters") != 0)
                    mnto->handle(prp.first, prp.second); 
            }
            result = std::move(fact->second->create_configurable(mnto));
        }
    }
    if (!result)
        throw writer_creation_exception(name);
    writer* wrt = dynamic_cast<writer*>(result.get());
    assert(wrt != nullptr);
    add_filters(*wrt, wrt_props);
    return std::move(result);
}

void config_file_configurator::log4cplus_properties_processor::fill_file_writer_memento(std::unique_ptr<memento>& mnto,
                                                                                        std::unique_ptr<formatter>&& fmt,
                                                                                        const properties& props)
{
    if (fmt) 
        mnto->handle(std::move(fmt));
    auto prop = props.get_one("Append");
    if (prop)
        mnto->handle("Append", *prop);
    prop = props.get_one("File");
    if (prop)
        mnto->handle("File", *prop);
    prop = props.get_one("ImmediateFlush");
    if (prop)
        mnto->handle("ImmediateFlush", *prop);
}

void config_file_configurator::log4cplus_properties_processor::process(const properties& props)
{
    auto lprops = props.get_subset("log4cplus.");
    auto root = lprops.get_one("rootLogger"); 
    if (root)
        create_logger("", *root, lprops);
    auto loggers = lprops.get_subset("logger.");
    for (const properties::const_iterator::value_type& lgr : loggers)
    {
        try
        {
            create_logger(lgr.first, lgr.second, lprops);
        }
        catch (std::exception& e)
        {
            cfg_.report_error("[logger " + lgr.first +
                "] An error occurred while processing the log4cplus config file: " +
                e.what());
        }
    }
}

std::vector<std::string> config_file_configurator::log4cplus_properties_processor::split_logger_descriptor(const std::string& desc)
{
    std::vector<std::string> result;
    regex::expression re("[ \t]*([^ \t,]+)");
    regex::iterator end;
    for (regex::iterator itor(desc, re); itor != end; ++itor)
    {
        regex::match mch = *itor;
        result.push_back(desc.substr(mch[1].begin(), mch[1].length()));
    }
    return result;
}

std::string config_file_configurator::log4cplus_properties_processor::time_pattern_from_log4cplus_schedule(const optional<std::string>& sched,
                                                                                                           const std::string& file_name)
{
    std::string result = file_name + ".%d{%Y-%m-%d}";
    if (sched)
    {
        auto low = text_util::to_lower(*sched);
        if (low == "monthly")
            result = file_name + ".%d{%Y-%m}";
        else if (low == "weekly")
            result = file_name + ".%d{%Y-%U}";
        else if (low == "hourly")
            result = file_name + ".%d{%Y-%m-%d %H}";
        else if (low == "minutely")
            result = file_name + ".%d{%Y-%m-%d %H:%M}";
        else if (low != "daily" && low != "twice_daily")
            cfg_.report_warning("The schedule is not valid: " + *sched);
    }
    else
    {
        cfg_.report_warning("The schedule has not been set");
    }
    return result;
}

#endif

}
