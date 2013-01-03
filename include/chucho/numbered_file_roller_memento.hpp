#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class CHUCHO_EXPORT numbered_file_roller_memento : public memento
{
public:
    numbered_file_roller_memento(const configurator& cfg);

    const optional<int>& get_max_index() const;
    const optional<int>& get_min_index() const;

private:
    optional<int> min_index_;
    optional<int> max_index_;
};

inline const optional<int>& numbered_file_roller_memento::get_max_index() const
{
    return max_index_;
}

inline const optional<int>& numbered_file_roller_memento::get_min_index() const
{
    return min_index_;
}

}

#endif
