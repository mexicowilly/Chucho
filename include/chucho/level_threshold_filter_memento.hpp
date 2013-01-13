#if !defined(CHUCHO_LEVEL_THRESHOLD_FILTER_MEMENTO_HPP__)
#define CHUCHO_LEVEL_THRESHOLD_FILTER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/level.hpp>

namespace chucho
{

class level_threshold_filter_memento : public memento
{
public:
    level_threshold_filter_memento(const configurator& cfg);

    std::shared_ptr<level> get_level() const;

private:
    std::shared_ptr<level> level_;
};

inline std::shared_ptr<level> level_threshold_filter_memento::get_level() const
{
    return level_;
}

}

#endif