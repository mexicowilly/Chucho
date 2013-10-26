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
#include <assert.h>

namespace chucho
{

config_file_configurator::config_file_configurator()
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
        {
            report_warning("The given properties have nothing that Chucho can use");
            return;
        }
        else
        {
            proc.reset(new log4cplus_properties_processor(*this));
        }

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
        else 
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
    auto loggers = props.get("logger");
    assert(get_factories().find("chucho::logger") != get_factories().end());
    auto lgr_fact = get_factories().find("chucho::logger")->second;
    while (loggers.first != loggers.second)
    {
        auto mnto = lgr_fact->create_memento(cfg_);
        properties cur_props = props.get_subset("logger." + loggers.first->second + '.');
        for (auto cur : cur_props)
        {
            if (cur.first == "writer")
                mnto->handle(create_writer(cur.second, props));
            else
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
}

bool config_file_configurator::log4cplus_properties_processor::boolean_value(const std::string& text) const
{
    std::string low;
    std::transform(text.begin(),
                   text.end(),
                   std::back_inserter(low),
                   [] (char c) { return std::tolower(c); });
    return low == "0" || low == "false" ? false : true;
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

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_file_writer(std::shared_ptr<formatter> fmt,
                                                                                                           const properties& props)
{
    assert(cfg_.get_factories().find("chucho::file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::file_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    mnto->handle(fmt);
    auto prop = props.get_one("Append");
    std::string open_mode = "truncate";
    if (prop && boolean_value(*prop))
        open_mode = "append";
    mnto->handle("on_start", open_mode);
    prop = props.get_one("File");
    if (prop)
        mnto->handle("file_name", *prop);
    prop = props.get_one("ImmediateFlush");
    if (prop)
        mnto->handle("flush", *prop);
    return fact->create_configurable(mnto);
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
            mnto->handle(create_writer(tok, props));
    }
    return lgr_fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_numbered_rolling_writer(std::shared_ptr<formatter> fmt,
                                                                                                                       const properties& props)
{
    assert(cfg_.get_factories().find("chucho::rolling_file_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::rolling_file_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    mnto->handle(fmt);
    auto prop = props.get_one("Append");
    std::string open_mode = "truncate";
    if (prop && boolean_value(*prop))
        open_mode = "append";
    mnto->handle("on_start", open_mode);
    prop = props.get_one("File");
    if (prop)
        mnto->handle("file_name", *prop);
    prop = props.get_one("ImmediateFlush");
    if (prop)
        mnto->handle("flush", *prop);
    assert(cfg_.get_factories().find("chucho::numbered_file_roller") != cfg_.get_factories().end()); 
    auto roller_fact = cfg_.get_factories().find("chucho::numbered_file_roller")->second;
    auto roller_mnto = roller_fact->create_memento(cfg_);
    prop = props.get_one("MaxBackupIndex");
    if (prop)
        roller_mnto->handle("max_index", *prop);
    mnto->handle(roller_fact->create_configurable(roller_mnto));
    assert(cfg_.get_factories().find("chucho::size_file_roll_trigger") != cfg_.get_factories().end()); 
    auto trigger_fact = cfg_.get_factories().find("chucho::size_file_roll_trigger")->second;
    auto trigger_mnto = trigger_fact->create_memento(cfg_);
    prop = props.get_one("MaxFileSize");
    if (prop)
        trigger_mnto->handle("max_size", *prop);
    mnto->handle(trigger_fact->create_configurable(trigger_mnto));
    return fact->create_configurable(mnto); 
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_remote_writer(const properties& props)
{
    assert(cfg_.get_factories().find("chucho::remote_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::remote_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    auto prop = props.get_one("host");
    if (prop)
        mnto->handle("host", *prop);
    prop = props.get_one("port");
    if (prop)
        mnto->handle("port", *prop);
    return fact->create_configurable(mnto);
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_syslog_writer(std::shared_ptr<formatter> fmt,
                                                                                                             const properties& props)
{
    assert(cfg_.get_factories().find("chucho::syslog_writer") != cfg_.get_factories().end());
    auto fact = cfg_.get_factories().find("chucho::syslog_writer")->second;
    auto mnto = fact->create_memento(cfg_);
    mnto->handle(fmt);
    auto prop = props.get_one("facility");
    if (prop)
        mnto->handle("facility", *prop);
    prop = props.get_one("host");
    if (prop)
        mnto->handle("host_name", *prop);
    return fact->create_configurable(mnto);
}

std::shared_ptr<configurable> config_file_configurator::log4cplus_properties_processor::create_writer(const std::string& name,
                                                                                                      const properties& props)
{
    std::shared_ptr<configurable> result;
    auto type = props.get_one("log4cplus.appender." + name);
    if (!type)
        return result;
    auto wrt_props = props.get_subset("log4cplus.appender." + name + "."); 
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
    else if (*type == "log4cplus::FileAppender")
    {
        result = create_file_writer(fmt, wrt_props);
    }
    else if (*type == "log4cplus::RollingFileAppender")
    {
        result = create_numbered_rolling_writer(fmt, wrt_props);
    }
    else if (*type == "log4cplus::SocketAppender")
    {
        result = create_remote_writer(wrt_props);
    }
    else if (*type == "log4cplus::SysLogAppender")
    {
        result = create_syslog_writer(fmt, wrt_props);
    }
    return result; 
}

void config_file_configurator::log4cplus_properties_processor::process(const properties& props)
{
    auto root = props.get_one("rootLogger");
    if (root)
        create_logger("", *root, props);
    auto loggers = props.get("logger");
    while (loggers.first != loggers.second)
    {
        create_logger(loggers.first->first, loggers.first->second, props);
        ++loggers.first;
    }
}

std::vector<std::string> config_file_configurator::log4cplus_properties_processor::split_logger_descriptor(const std::string& desc)
{
    std::vector<std::string> result;
    regex::expression re("[ \\t]*([^ \\t,]+)");
    regex::iterator end;
    for (regex::iterator itor(desc, re); itor != end; ++itor)
    {
        regex::match mch = *itor;
        result.push_back(desc.substr(mch[1].begin(), mch[1].length()));
    }
    return result;
}

}
