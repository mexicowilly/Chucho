#if !defined(CHUCHO_YAML_CONFIGURATOR_HPP__)
#define CHUCHO_YAML_CONFIGURATOR_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/configurator.hpp>
#include <yaml-cpp/yaml.h>

namespace chucho
{

class memento;

class yaml_configurator : public configurator
{
public:
    yaml_configurator();

    virtual void configure(std::istream& in) override;

private:
    std::map<std::string, std::string> extract_variables(const YAML::Node& n);
    void handle(const std::string& key, const YAML::Node& n, std::shared_ptr<memento> mnto);
};

}

#endif
