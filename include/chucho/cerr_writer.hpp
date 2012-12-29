#if !defined(CHUCHO_COUT_WRITER_HPP__)
#define CHUCHO_COUT_WRITER_HPP__

#include <chucho/console_writer.hpp>

namespace chucho
{

class cerr_writer_memento;

class CHUCHO_EXPORT cerr_writer : public console_writer
{
public:
    cerr_writer(std::shared_ptr<formatter> fmt);
    cerr_writer(const cerr_writer_memento& mnto);
};

}

#endif
