#if !defined(CHUCHO_CERR_WRITER_MEMENTO_HPP__)
#define CHUCHO_CERR_WRITER_MEMENTO_HPP__

#include <chucho/writer_memento.hpp>

namespace chucho
{

class CHUCHO_EXPORT cerr_writer_memento : public writer_memento
{
public:
    cerr_writer_memento(const configurator& cfg);
};

}

#endif
