#if !defined(CHUCHO_CONFIGURABLE_FACTORY_HPP__)
#define CHUCHO_CONFIGURABLE_FACTORY_HPP__

#include <chucho/memento.hpp>
#include <chucho/named_configurable.hpp>
#include <chucho/configurator.hpp>
#include <chucho/status_reporter.hpp>
#include <memory>

namespace chucho
{

class CHUCHO_EXPORT configurable_factory : public status_reporter
{
public:
    configurable_factory();
    virtual ~configurable_factory();

    virtual named_configurable create_configurable(std::shared_ptr<memento> mnto) = 0;
    virtual std::shared_ptr<memento> create_memento(const configurator& cfg) = 0;
};

}

#endif
