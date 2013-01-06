#include <chucho/cout_writer.hpp>
#include <iostream>

namespace chucho
{

cout_writer::cout_writer(std::shared_ptr<formatter> fmt)
    : console_writer(fmt, std::cout)
{
    set_status_origin("cout_writer");
}

}
