#if !defined(CHUCHO_NAMED_CONFIGURABLE_HPP__)
#define CHUCHO_NAMED_CONFIGURABLE_HPP__

#include <chucho/configurable.hpp>

namespace chucho
{

class CHUCHO_EXPORT named_configurable
{
public:
    named_configurable(const std::string& name,
                       std::shared_ptr<configurable> cnf);

    std::shared_ptr<configurable> get_configurable() const;
    const std::string& get_name() const;

private:
    std::string name_;
    std::shared_ptr<configurable> cnf_;
};

inline named_configurable::named_configurable(const std::string& name,
                                              std::shared_ptr<configurable> cnf)
    : name_(name),
      cnf_(cnf)
{
}

inline std::shared_ptr<configurable> named_configurable::get_configurable() const
{
    return cnf_;
}

inline const std::string& named_configurable::get_name() const
{
    return name_;
}

}

#endif
