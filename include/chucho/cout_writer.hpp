#if !defined(CHUCHO_COUT_WRITER_HPP__)
#define CHUCHO_COUT_WRITER_HPP__

#include <chucho/console_writer.hpp>

namespace chucho
{

class cout_writer_memento;

class CHUCHO_EXPORT cout_writer : public console_writer
{
public:
    cout_writer(std::shared_ptr<formatter> fmt);
    cout_writer(const cout_writer_memento& mnto);
};

}

#endif
