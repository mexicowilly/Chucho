#include <chucho/cout_writer.hpp>
#include <chucho/cout_writer_memento.hpp>
#include <iostream>

namespace chucho
{

cout_writer::cout_writer(std::shared_ptr<formatter> fmt)
    : console_writer(fmt, std::cout)
{
    set_status_origin("cout_writer");
}

cout_writer::cout_writer(const cout_writer_memento& mnto)
    : cout_writer(mnto.get_formatter())
{
}

}
