#if !defined(CHUCHO_LEVEL_FILTER_MEMENTO_HPP__)
#define CHUCHO_LEVEL_FILTER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/filter.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class level_filter_memento : public memento
{
public:
    level_filter_memento(const configurator& cfg);

    std::shared_ptr<level> get_level() const;
    const optional<filter::result>& get_on_match() const;
    const optional<filter::result>& get_on_mismatch() const;

private:
    std::shared_ptr<level> level_;
    optional<filter::result> on_match_;
    optional<filter::result> on_mismatch_;
};

inline std::shared_ptr<level> level_filter_memento::get_level() const
{
    return level_;
}

inline const optional<filter::result>& level_filter_memento::get_on_match() const
{
    return on_match_;
}

inline const optional<filter::result>& level_filter_memento::get_on_mismatch() const
{
    return on_mismatch_;
}

}

#endif