#include <chucho/console_writer.hpp>
#include <iostream>

namespace chucho
{

console_writer::console_writer(std::ostream& stream)
    : stream_(stream)
{
}

void console_writer::write_impl(const event& evt)
{
    stream_ << formatter_->format(evt);
}

cout_writer::cout_writer()
    : console_writer(std::cout)
{
}

cerr_writer::cerr_writer()
    : console_writer(std::cerr)
{
}

}
