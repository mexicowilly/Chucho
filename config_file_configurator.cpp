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
#include <assert.h>

#include <iostream>

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

}

namespace chucho
{

config_file_configurator::config_file_configurator()
    : memento_key_set_(memento_key_set::CHUCHO)
{
    set_status_origin("config_file_configurator");
}

void config_file_configurator::configure(std::istream& in)
{
    std::unique_ptr<properties_processor> proc;
    properties props(in);
    std::unique_ptr<properties> id(new properties(props.get_subset("chucho.")));
    if (!id->empty())
    {
        proc.reset(new chucho_properties_processor(*this));
    }
    else
    {
        id.reset(new properties(props.get_subset("log4cplus.")));
        if (id->empty())
            throw exception("Found neither Chucho nor log4cplus keys in the configuration");
        proc.reset(new log4cplus_properties_processor(*this));
        memento_key_set_ = memento_key_set::LOG4CPLUS;
    }
    id.reset();
    proc->process(props);
}

config_file_configurator::properties_processor::properties_processor(config_file_configurator& cfg)
    : cfg_(cfg)
{
}

config_file_configurator::chucho_properties_processor::chucho_properties_processor(config_file_configurator& cfg)
    : properties_processor(cfg)
{
    cfg_.report_info("Using Chucho-style config file configuration");
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_file_compressor(const std::string& name,
                                                                                                            const properties& props)
{
    std::string type("file_compressor");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
        mnto->handle(cur.first, cur.second);
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_file_roller(const std::string& name,
                                                                                                        const properties& props)
{
    std::string type("file_roller");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
    {
        if (cur.first == "file_compressor")
            mnto->handle(create_file_compressor(cur.second, props));
        else 
            mnto->handle(cur.first, cur.second);
    }
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_file_roll_trigger(const std::string& name,
                                                                                                              const properties& props)
{
    std::string type("file_roll_trigger");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
        mnto->handle(cur.first, cur.second);
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_filter(const std::string& name,
                                                                                                   const properties& props)
{
    std::string type("filter");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
        mnto->handle(cur.first, cur.second);
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_formatter(const std::string& name,
                                                                                                      const properties& props)
{
    std::string type("formatter");
    auto fact = get_factory(type, name, props);
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
        mnto->handle(cur.first, cur.second);
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::chucho_properties_processor::create_writer(const std::string& name,
                                                                                                   const properties& props)
{
    std::string type("writer");
    auto fact = get_factory(type, name, props); 
    auto mnto = fact->create_memento(cfg_);
    auto cur_props = props.get_subset(type + '.' + name + '.');
    for (auto cur : cur_props)
    {
        if (cur.first == "filter")
            mnto->handle(create_filter(cur.second, props));
        else if (cur.first == "formatter")
            mnto->handle(create_formatter(cur.second, props));
        else if (cur.first == "file_roller")
            mnto->handle(create_file_roller(cur.second, props));
        else if (cur.first == "file_roll_trigger")
            mnto->handle(create_file_roll_trigger(cur.second, props));
        else if (cur.first == "writer")
            mnto->handle(create_writer(cur.second, props));
        else if (cur.first.find('.') == std::string::npos)
            mnto->handle(cur.first, cur.second); 
    }
    return fact->create_configurable(mnto); 
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
        ++loggers.first;
    }
}

config_file_configurator::log4cplus_properties_processor::log4cplus_properties_processor(config_file_configurator& cfg)
    : properties_processor(cfg)
{
    cfg_.report_info("Using log4cplus-style config file configuration");
    factory_keys_["log4cplus::FileAppender"] = "chucho::file_writer";
    factory_keys_["log4cplus::SocketAppender"] = "chucho::remote_writer";
    factory_keys_["log4cplus::SysLogAppender"] = "chucho::syslog_writer";
    factory_keys_["log4cplus::spi::LogLevelMatchFilter"] = "chucho::level_filter";
    factory_keys_["log4cplus::spi::LogLevelRangeFilter"] = "chucho::level_threshold_filter";
}

void config_file_configurator::log4cplus_properties_processor::add_filters(std::shared_ptr<writer> wrt,
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
            auto flt = std::make_shared<level_threshold_filter>(level::OFF_());
            wrt->add_filter(flt);
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
                auto mnto = fact->second->create_memento(cfg_);
                auto flt_props = flts.get_subset(name + ".");
                for (auto prp : flt_props)
                {
                    if (prp.second.find(*type) != 0)
                        mnto->handle(prp.first, prp.second); 
                }
                wrt->add_filter(std::dynamic_pointer_cast<filter>(fact->second->create_configurable(mnto)));
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

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_async_writer(const properties& top_props,
                                                                                                            const properties& wrt_props)
{
    assert(cfg_.get_factories().find("chucho::async_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::async_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    auto app = wrt_props.get_one("Appender");
    if (app)
        mnto->handle(create_writer(*app, top_props));
    return fact->create_configurable(mnto);
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_console_writer(std::shared_ptr<formatter> fmt,
                                                                                                              const properties& props)
{
    auto type = props.get_one("logToStdErr");
    std::shared_ptr<configurable> result;
    if (type && boolean_value(*type))
        result = std::make_shared<cerr_writer>(fmt);
    else
        result = std::make_shared<cout_writer>(fmt);
    return result;
}

std::shared_ptr<formatter> config_file_configurator::log4cplus_properties_processor::create_formatter(const std::string& type,
                                                                                                      const properties& props)
{
    std::shared_ptr<formatter> result;
    if (type == "log4cplus::PatternLayout")
    {
        auto pat = props.get_one("ConversionPattern");
        if (pat)
            result = std::make_shared<pattern_formatter>(*pat); 
        else
            cfg_.report_error("A log4cplus::PatternLayout requires a ConversionPattern property");
    }
    else if (type == "log4cplus::SimpleLayout")
    {
        result = std::make_shared<pattern_formatter>("%p - %m%n");
    }
    else if (type == "log4cplus::TTCCLayout")
    {
        result = std::make_shared<pattern_formatter>("%r %t %p %c - %m%n");
    }
    return result;
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_logger(const std::string& name,
                                                                                                      const std::string& desc,
                                                                                                      const properties& props)
{
    assert(get_factories().find("chucho::logger") != get_factories().end());
    auto lgr_fact = get_factories().find("chucho::logger")->second;
    auto mnto = lgr_fact->create_memento(cfg_);
    mnto->handle("name", name);
    auto tokens = split_logger_descriptor(desc);
    if (!tokens.empty())
        mnto->handle("level", tokens[0]);
    tokens.erase(tokens.begin());
    if (!tokens.empty())
    {
        for (auto tok : tokens)
        {
            try
            {
                auto wrt = create_writer(tok, props);
                if (wrt)
                    mnto->handle(wrt);
            }
            catch (writer_creation_exception& e)
            {
                cfg_.report_warning(e.what());
            }
        }
    }
    return lgr_fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_size_rolling_writer(std::shared_ptr<formatter> fmt,
                                                                                                                   const properties& props)
{
    assert(cfg_.get_factories().find("chucho::rolling_file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::rolling_file_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    fill_file_writer_memento(mnto, fmt, props);
    assert(cfg_.get_factories().find("chucho::numbered_file_roller") != cfg_.get_factories().end()); 
    auto roller_fact = cfg_.get_factories().find("chucho::numbered_file_roller")->second;
    auto roller_mnto = roller_fact->create_memento(cfg_);
    auto prop = props.get_one("MaxBackupIndex");
    roller_mnto->handle("max_index", (prop ? *prop : "1"));
    mnto->handle(roller_fact->create_configurable(roller_mnto));
    assert(cfg_.get_factories().find("chucho::size_file_roll_trigger") != cfg_.get_factories().end()); 
    auto trigger_fact = cfg_.get_factories().find("chucho::size_file_roll_trigger")->second;
    auto trigger_mnto = trigger_fact->create_memento(cfg_);
    prop = props.get_one("MaxFileSize");
    trigger_mnto->handle("max_size", (prop ? *prop : "10m"));
    mnto->handle(trigger_fact->create_configurable(trigger_mnto));
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_time_rolling_writer(std::shared_ptr<formatter> fmt,
                                                                                                                   const properties& props)
{
    assert(cfg_.get_factories().find("chucho::rolling_file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::rolling_file_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    fill_file_writer_memento(mnto, fmt, props);
    assert(cfg_.get_factories().find("chucho::time_file_roller") != cfg_.get_factories().end()); 
    auto roller_fact = cfg_.get_factories().find("chucho::time_file_roller")->second;
    auto roller_mnto = roller_fact->create_memento(cfg_);
    auto prop = props.get_one("MaxBackupIndex");
    roller_mnto->handle("max_history", prop ? *prop : "10");
    std::string fn = std::dynamic_pointer_cast<file_writer_memento>(mnto)->get_file_name();
    roller_mnto->handle("file_name_pattern", time_pattern_from_log4cplus_schedule(props.get_one("Schedule"), fn));
    mnto->handle(roller_fact->create_configurable(roller_mnto));
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_writer(const std::string& name,
                                                                                                      const properties& props)
{
    std::shared_ptr<configurable> result;
    auto type = props.get_one("appender." + name); 
    if (!type)
        return result;
    auto wrt_props = props.get_subset("appender." + name + "."); 
    auto fmt_type = wrt_props.get_one("layout"); 
    std::shared_ptr<formatter> fmt;
    if (fmt_type)
    {
        auto fmt_props = wrt_props.get_subset("layout.");
        fmt = create_formatter(*fmt_type, fmt_props);
    }
    if (*type == "log4cplus::ConsoleAppender") 
    {
        result = create_console_writer(fmt, wrt_props);
    }
    else if (*type == "log4cplus::RollingFileAppender")
    {
        result = create_size_rolling_writer(fmt, wrt_props);
    }
    else if (*type == "log4cplus::DailyRollingFileAppender")
    {
        result = create_time_rolling_writer(fmt, wrt_props);
    }
    else if (*type == "log4cplus::AsyncAppender")
    {
        result = create_async_writer(props, wrt_props);
    }
    else if (*type == "log4cplus::NullAppender")
    {
        return result;
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
            auto mnto = fact->second->create_memento(cfg_);
            if (fmt)
                mnto->handle(fmt); 
            for (auto prp : wrt_props)
            {
                if (prp.first.find("layout") != 0 && prp.first.find("filters") != 0)
                    mnto->handle(prp.first, prp.second); 
            }
            result = fact->second->create_configurable(mnto);
        }
    }
    if (!result)
        throw writer_creation_exception(name);
    add_filters(std::dynamic_pointer_cast<writer>(result), wrt_props); 
    return result; 
}

void config_file_configurator::log4cplus_properties_processor::fill_file_writer_memento(std::shared_ptr<memento> mnto,
                                                                                        std::shared_ptr<formatter> fmt,
                                                                                        const properties& props)
{
    mnto->handle(fmt);
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
    /*
    for (auto p : lprops)
        std::cout << p.first << '=' << p.second << std::endl;
    */
    auto root = lprops.get_one("rootLogger"); 
    if (root)
        create_logger("", *root, lprops);
    auto loggers = lprops.get_subset("logger.");
    for (const properties::const_iterator::value_type& lgr : loggers)
        create_logger(lgr.first, lgr.second, lprops);
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

}
