#if !defined(CHUCHO_LOGGER_MEMENTO_HPP__)
#define CHUCHO_LOGGER_MEMENTO_HPP__

#include <chucho/memento.hpp>
#include <chucho/level.hpp>
#include <chucho/writer.hpp>
#include <vector>

namespace chucho
{

class CHUCHO_EXPORT logger_memento : public memento
{
public:
    logger_memento(const configurator& cfg);

    std::shared_ptr<level> get_level() const;
    const std::string& get_name() const;
    const std::vector<std::shared_ptr<writer>> get_writers() const;
    std::shared_ptr<bool> get_writes_to_ancestors() const;

private:
    std::string name_;
    std::shared_ptr<level> level_;
    std::vector<std::shared_ptr<writer>> writers_;
    std::shared_ptr<bool> writes_to_ancestors_;
};

inline std::shared_ptr<level> logger_memento::get_level() const
{
    return level_;
}

inline const std::string& logger_memento::get_name() const
{
    return name_;
}

inline const std::vector<std::shared_ptr<writer>> logger_memento::get_writers() const
{
    return writers_;
}

inline std::shared_ptr<bool> logger_memento::get_writes_to_ancestors() const
{
    return writes_to_ancestors_;
}

}

#endif
