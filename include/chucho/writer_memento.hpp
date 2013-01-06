#if !defined(CHUCHO_WRITER_MEMENTO_HPP__)
#define CHUCHO_WRITER_MEMENTO_HPP__

#include <chucho/memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT writer_memento : public memento
{
public:
    writer_memento(const configurator& cfg);

    const std::vector<std::shared_ptr<filter>>& get_filters() const;
    std::shared_ptr<formatter> get_formatter() const;
    virtual void handle(std::shared_ptr<configurable> cnf) override;

private:
    std::shared_ptr<formatter> fmt_;
    std::vector<std::shared_ptr<filter>> filters_;
};

inline const std::vector<std::shared_ptr<filter>>& writer_memento::get_filters() const
{
    return filters_;
}

inline std::shared_ptr<formatter> writer_memento::get_formatter() const
{
    return fmt_;
}

}

#endif
