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

}
