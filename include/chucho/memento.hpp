#if !defined(CHUCHO_MEMENTO_HPP__)
#define CHUCHO_MEMENTO_HPP__

#include <chucho/configurator.hpp>
#include <chucho/configurable.hpp>
#include <functional>

namespace chucho
{

class CHUCHO_EXPORT memento : public status_reporter
{
public:
    memento(const configurator& cfg);

    void handle(const std::string& key, const std::string& value);
    virtual void handle(std::shared_ptr<configurable> cnf);

protected:
    typedef std::function<void(const std::string&)> handler;

    bool boolean_value(const std::string& value);
    void set_handler(const std::string& key, handler hand);
    std::string to_lower(const std::string& value);

    const configurator& cfg_;

private:
    std::map<std::string, handler> handlers_;
};

inline void memento::set_handler(const std::string& key, handler hand)
{
    handlers_[key] = hand;
}

}

#endif
