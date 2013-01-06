#if !defined(CHUCHO_CONFIGURATOR_HPP__)
#define CHUCHO_CONFIGURATOR_HPP__

#include <chucho/status_reporter.hpp>
#include <chucho/logger.hpp>
#include <chucho/named_configurable.hpp>
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
    std::shared_ptr<filter> get_filter(const std::string& name) const;
    std::shared_ptr<formatter> get_formatter(const std::string& name) const;
    std::shared_ptr<writer> get_writer(const std::string& name) const;

protected:
    static std::map<std::string, std::shared_ptr<configurable_factory>>& get_factories();

    void add_variables(const std::map<std::string, std::string>& vars);
    void handle(const named_configurable& cnf);
    std::string resolve_variables(const std::string& val);

private:
    std::map<std::string, std::shared_ptr<formatter>> formatters_;
    std::map<std::string, std::shared_ptr<writer>> writers_;
    std::map<std::string, std::string> variables_;
    std::map<std::string, std::shared_ptr<filter>> filters_;
};

#define CHUCHO_REGISTER_CONFIGURABLE_FACTORY(ns, nm) \
    namespace \
    { \
    struct nm##_registrar \
    { \
        nm##_registrar() \
        { \
            std::shared_ptr<chucho::nm> fact(new ns::nm()); \
            chucho::configurator::add_configurable_factory(#ns"::"#nm, fact); \
        } \
    } chucho_##nm##_reg; \
    }

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
