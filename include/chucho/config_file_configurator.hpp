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

#if !defined(CHUCHO_CONFIG_FILE_CONFIGURATOR_HPP__)
#define CHUCHO_CONFIG_FILE_CONFIGURATOR_HPP__

#if !defined(CHUCHO_BUILD)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>
#include <chucho/configurable.hpp>
#include <map>
#include <vector>

namespace chucho
{

class properties;
class formatter;

class CHUCHO_EXPORT config_file_configurator : public configurator
{
public:
    config_file_configurator();

    virtual void configure(std::istream& in) override;

private:
    class properties_processor
    {
    public:
        properties_processor(config_file_configurator& cfg);

        virtual void process(const properties& props) = 0;

    protected:
        config_file_configurator& cfg_;
    };

    class chucho_properties_processor : public properties_processor
    {
    public:
        chucho_properties_processor(config_file_configurator& cfg);

        virtual void process(const properties& props) override;

    private:
        std::shared_ptr<configurable> create_file_compressor(const std::string& name,
                                                             const properties& props);
        std::shared_ptr<configurable> create_file_roller(const std::string& name,
                                                         const properties& props);
        std::shared_ptr<configurable> create_file_roll_trigger(const std::string& name,
                                                               const properties& props);
        std::shared_ptr<configurable> create_filter(const std::string& name,
                                                    const properties& props);
        std::shared_ptr<configurable> create_formatter(const std::string& name,
                                                       const properties& props);
        std::shared_ptr<configurable> create_writer(const std::string& name,
                                                    const properties& props);
        std::shared_ptr<configurable_factory> get_factory(const std::string& type,
                                                          const std::string& name,
                                                          const properties& props);
    };

    class log4cplus_properties_processor : public properties_processor
    {
    public:
        log4cplus_properties_processor(config_file_configurator& cfg);

        virtual void process(const properties& props) override;

    private:
        bool boolean_value(const std::string& text) const;
        std::shared_ptr<configurable> create_console_writer(std::shared_ptr<formatter> fmt,
                                                            const properties& props);
        std::shared_ptr<configurable> create_file_writer(std::shared_ptr<formatter> fmt,
                                                         const properties& props);
        std::shared_ptr<formatter> create_formatter(const std::string& type,
                                                    const properties& props);
        std::shared_ptr<configurable> create_logger(const std::string& name,
                                                    const std::string& desc,
                                                    const properties& props);
        std::shared_ptr<configurable> create_numbered_rolling_writer(std::shared_ptr<formatter> fmt,
                                                                     const properties& props);
        std::shared_ptr<configurable> create_remote_writer(const properties& props);
        std::shared_ptr<configurable> create_syslog_writer(std::shared_ptr<formatter> fmt,
                                                           const properties& props);
        std::shared_ptr<configurable> create_writer(const std::string& name,
                                                    const properties& props);
        std::vector<std::string> split_logger_descriptor(const std::string& desc);
    };
};

}

#endif
