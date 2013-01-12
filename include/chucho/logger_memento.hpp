#if !defined(CHUCHO_LOGGER_MEMENTO_HPP__)
#define CHUCHO_LOGGER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/level.hpp>
#include <chucho/writer.hpp>
#include <chucho/optional.hpp>
#include <vector>

namespace chucho
{

class logger_memento : public memento
{
public:
    logger_memento(const configurator& cfg);

    std::shared_ptr<level> get_level() const;
    const optional<std::string>& get_name() const;
    const std::vector<std::shared_ptr<writer>>& get_writers() const;
    const optional<bool>& get_writes_to_ancestors() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    optional<std::string> name_;
    std::shared_ptr<level> level_;
    std::vector<std::shared_ptr<writer>> writers_;
    optional<bool> writes_to_ancestors_;
};

inline std::shared_ptr<level> logger_memento::get_level() const
{
    return level_;
}

inline const optional<std::string>& logger_memento::get_name() const
{
    return name_;
}

inline const std::vector<std::shared_ptr<writer>>& logger_memento::get_writers() const
{
    return writers_;
}

inline const optional<bool>& logger_memento::get_writes_to_ancestors() const
{
    return writes_to_ancestors_;
}

}

#endif
