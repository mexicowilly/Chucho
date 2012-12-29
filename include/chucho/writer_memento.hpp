#if !defined(CHUCHO_WRITER_MEMENTO_HPP__)
#define CHUCHO_WRITER_MEMENTO_HPP__

#include <chucho/memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT writer_memento : public memento
{
public:
    writer_memento(const configurator& cfg);

    std::shared_ptr<formatter> get_formatter() const;

private:
    std::shared_ptr<formatter> fmt_;
};

inline std::shared_ptr<formatter> writer_memento::get_formatter() const
{
    return fmt_;
}

}

#endif
