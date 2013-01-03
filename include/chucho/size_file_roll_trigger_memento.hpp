#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class CHUCHO_EXPORT size_file_roll_trigger_memento : public memento
{
public:
    size_file_roll_trigger_memento(const configurator& cfg);

    const optional<unsigned long long>& get_max_size() const;

private:
    optional<unsigned long long> max_size_;
};

inline const optional<unsigned long long>& size_file_roll_trigger_memento::get_max_size() const
{
    return max_size_;
}

}

#endif
