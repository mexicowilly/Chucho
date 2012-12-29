#include <chucho/file_roller.hpp>
#include <chucho/file_writer.hpp>

namespace chucho
{

file_roller::file_roller()
    : file_writer_(nullptr)
{
}

void file_roller::set_file_writer(file_writer& file_writer)
{
    file_writer_ = &file_writer;
}

}
