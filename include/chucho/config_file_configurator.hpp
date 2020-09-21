/*
 * Copyright 2013-2020 Will Mason
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
#include <chucho/memento.hpp>
#include <chucho/memento_key_set.hpp>
#include <chucho/optional.hpp>
#include <chucho/writer.hpp>
#include <map>
#include <vector>

namespace chucho
{

class properties;
class formatter;

class CHUCHO_PRIV_EXPORT config_file_configurator : public configurator
{
public:
    config_file_configurator(security_policy& sec_pol);

    virtual void configure(std::istream& in) override;
    memento_key_set get_memento_key_set() const;

private:
    class properties_processor
    {
    public:
        properties_processor(config_file_configurator& cfg);
        virtual ~properties_processor() { }

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
        std::unique_ptr<configurable> create_configurable(const std::string& type,
                                                          const std::string& name,
                                                          const properties& props);

        std::unique_ptr<configurable> create_file_roller(const std::string& name,
                                                         const properties& props);
        std::unique_ptr<configurable> create_writer(const std::string& name,
                                                    const properties& props);
        configurable_factory& get_factory(const std::string& type,
                                          const std::string& name,
                                          const properties& props);
    };

    class log4cplus_properties_processor : public properties_processor
    {
    public:
        log4cplus_properties_processor(config_file_configurator& cfg);

        virtual void process(const properties& props) override;

    private:
        void add_filters(writer& wrt, const properties& props);
        bool boolean_value(const std::string& text) const;
        std::unique_ptr<configurable> create_async_writer(const properties& top_props,
                                                          const properties& wrt_props);
        std::unique_ptr<configurable> create_console_writer(std::unique_ptr<formatter>&& fmt,
                                                            const properties& props);
        std::unique_ptr<formatter> create_formatter(const std::string& type,
                                                    const properties& props);
        void create_logger(const std::string& name,
                           const std::string& desc,
                           const properties& props);
        std::unique_ptr<configurable> create_size_rolling_writer(std::unique_ptr<formatter>&& fmt,
                                                                 const properties& props);
        std::unique_ptr<configurable> create_time_rolling_writer(std::unique_ptr<formatter>&& fmt,
                                                                 const properties& props);
        std::unique_ptr<configurable> create_writer(const std::string& name,
                                                    const properties& props);
        void fill_file_writer_memento(std::unique_ptr<memento>& mnto,
                                      std::unique_ptr<formatter>&& fmt,
                                      const properties& props);
        std::vector<std::string> split_logger_descriptor(const std::string& desc);
        std::string time_pattern_from_log4cplus_schedule(const optional<std::string>& sched,
                                                         const std::string& file_name);

        std::map<std::string, std::string> factory_keys_;
    };

    memento_key_set memento_key_set_;
};

inline memento_key_set config_file_configurator::get_memento_key_set() const
{
    return memento_key_set_;
}

}

#endif
