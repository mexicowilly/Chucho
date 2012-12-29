#include <chucho/cerr_writer.hpp>
#include <chucho/cerr_writer_memento.hpp>
#include <iostream>

namespace chucho
{

cerr_writer::cerr_writer(std::shared_ptr<formatter> fmt)
    : console_writer(fmt, std::cerr)
{
    set_status_origin("cerr_writer");
}

cerr_writer::cerr_writer(const cerr_writer_memento& mnto)
    : cerr_writer(mnto.get_formatter())
{
}

}

