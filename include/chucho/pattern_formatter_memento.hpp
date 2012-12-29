#if !defined(CHUCHO_PATTERN_FORMATTER_MEMENTO_HPP__)
#define CHUCHO_PATTERN_FORMATTER_MEMENTO_HPP__

#include <chucho/memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT pattern_formatter_memento : public memento
{
public:
    pattern_formatter_memento(const configurator& cfg);

    const std::string& get_pattern() const;

private:
    std::string pattern_;
};

inline const std::string& pattern_formatter_memento::get_pattern() const
{
    return pattern_;
}

}

#endif
