#if !defined(CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__)
#define CHUCHO_NUMBERED_FILE_ROLLER_MEMENTO_HPP__

#include <chucho/memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT numbered_file_roller_memento : public memento
{
public:
    numbered_file_roller_memento(const configurator& cfg);

    std::shared_ptr<int> get_max_index() const;
    std::shared_ptr<int> get_min_index() const;

private:
    std::shared_ptr<int> min_index_;
    std::shared_ptr<int> max_index_;
};

inline std::shared_ptr<int> numbered_file_roller_memento::get_max_index() const
{
    return max_index_;
}

inline std::shared_ptr<int> numbered_file_roller_memento::get_min_index() const
{
    return min_index_;
}

}

#endif
