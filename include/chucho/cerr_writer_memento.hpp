#if !defined(CHUCHO_CERR_WRITER_MEMENTO_HPP__)
#define CHUCHO_CERR_WRITER_MEMENTO_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>

namespace chucho
{

class cerr_writer_memento : public writer_memento
{
public:
    cerr_writer_memento(const configurator& cfg);
};

}

#endif
