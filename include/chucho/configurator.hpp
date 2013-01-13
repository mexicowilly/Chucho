#if !defined(CHUCHO_CONFIGURATOR_HPP__)
#define CHUCHO_CONFIGURATOR_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/status_reporter.hpp>
#include <chucho/configuration.hpp>
#include <map>

namespace chucho
{

class configurable_factory;

class configurator : public status_reporter
{
public:
    static void add_configurable_factory(const std::string& name,
                                         std::shared_ptr<configurable_factory> fact);

    configurator();

    virtual void configure(std::istream& in) = 0;

protected:
    friend CHUCHO_NO_EXPORT void configuration::perform();

    static std::map<std::string, std::shared_ptr<configurable_factory>>& get_factories();

    void add_variables(const std::map<std::string, std::string>& vars);
    std::string resolve_variables(const std::string& val);

private:
    /**
     * This is invoked by configuration::perform().
     */
    static void initialize();

    std::map<std::string, std::string> variables_;
};

inline void configurator::add_configurable_factory(const std::string& name,
                                                   std::shared_ptr<configurable_factory> fact)
{
    get_factories()[name] = fact;
}

inline void configurator::add_variables(const std::map<std::string, std::string>& vars)
{
    variables_.insert(vars.begin(), vars.end());
}

}

#endif
