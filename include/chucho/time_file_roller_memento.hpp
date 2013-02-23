#if !defined(CHUCHO_TIME_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_TIME_FILE_ROLLER_MEMENTO_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class time_file_roller_memento : public memento
{
public:
    time_file_roller_memento(const configurator& cfg);

    const std::string& get_file_name_pattern() const;
    const optional<std::size_t>& get_max_history() const;

private:
    std::string file_name_pattern_;
    optional<std::size_t> max_history_;
};

inline const std::string& time_file_roller_memento::get_file_name_pattern() const
{
    return file_name_pattern_;
}

inline const optional<std::size_t>& time_file_roller_memento::get_max_history() const
{
    return max_history_;
}

}

#endif
