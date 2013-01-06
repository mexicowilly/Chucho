#if !defined(CHUCHO_COUT_WRITER_HPP__)
#define CHUCHO_COUT_WRITER_HPP__

#include <chucho/console_writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT cerr_writer : public console_writer
{
public:
    cerr_writer(std::shared_ptr<formatter> fmt);
};

}

#endif
