#include <chucho/file_roller.hpp>
#include <chucho/file_writer.hpp>

namespace chucho
{

void file_roller::set_file_writer(file_writer* file_writer)
{
    file_writer_ = file_writer;
}

}
