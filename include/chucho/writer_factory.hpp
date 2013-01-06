#if !defined(CHUCHO_WRITER_FACTORY_HPP__)
#define CHUCHO_WRITER_FACTORY_HPP__

#include <chucho/configurable_factory.hpp>
#include <chucho/writer_memento.hpp>

namespace chucho
{

class writer_factory : public configurable_factory
{
protected:
    void set_filters(std::shared_ptr<configurable> cnf, std::shared_ptr<writer_memento> mnto);
};

}

#endif
