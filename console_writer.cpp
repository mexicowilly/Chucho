#include <chucho/console_writer.hpp>
#include <iostream>

namespace chucho
{

console_writer::console_writer(std::shared_ptr<formatter> fmt,
                               std::ostream& stream)
    : writer(fmt),
      stream_(stream)
{
}

void console_writer::write_impl(const event& evt)
{
    stream_ << formatter_->format(evt);
    stream_.flush();
}

cout_writer::cout_writer(std::shared_ptr<formatter> fmt)
    : console_writer(fmt, std::cout)
{
    set_status_origin("cout_writer");
}

cerr_writer::cerr_writer(std::shared_ptr<formatter> fmt)
    : console_writer(fmt, std::cerr)
{
    set_status_origin("cerr_writer");
}

}
