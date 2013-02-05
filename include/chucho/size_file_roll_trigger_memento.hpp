#if !defined(CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__)
#define CHUCHO_SIZE_FILE_ROLL_TRIGGER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/memento.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class size_file_roll_trigger_memento : public memento
{
public:
    size_file_roll_trigger_memento(const configurator& cfg);

    const optional<unsigned long long>& get_max_size() const;

private:
    void parse(const std::string& spec);

    optional<unsigned long long> max_size_;
};

inline const optional<unsigned long long>& size_file_roll_trigger_memento::get_max_size() const
{
    return max_size_;
}

}

#endif
