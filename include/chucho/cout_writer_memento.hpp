#if !defined(CHUCHO_COUT_WRITER_MEMENTO_HPP__)
#define CHUCHO_COUT_WRITER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>

namespace chucho
{

class cout_writer_memento : public writer_memento
{
public:
    cout_writer_memento(const configurator& cfg);
};

}

#endif

