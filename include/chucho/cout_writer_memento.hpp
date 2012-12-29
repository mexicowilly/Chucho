#if !defined(CHUCHO_COUT_WRITER_MEMENTO_HPP__)
#define CHUCHO_COUT_WRITER_MEMENTO_HPP__

#include <chucho/writer_memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT cout_writer_memento : public writer_memento
{
public:
    cout_writer_memento(const configurator& cfg);
};

}

#endif

