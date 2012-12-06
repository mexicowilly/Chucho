#include <chucho/numbered_file_roller.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/file.hpp>
#include <chucho/exception.hpp>
#include <stdexcept>
#include <cstdio>
#include <sstream>

namespace chucho
{

numbered_file_roller::numbered_file_roller(int min_index, int max_index)
    : min_index_(min_index),
      max_index_(max_index)
{
    if (min_index > max_index)
        throw std::invalid_argument("numbered_file_roller: min_index must be less than or equal to max_index");
}

std::string numbered_file_roller::get_active_file_name()
{
    return file_writer_->get_file_name();
}

std::string numbered_file_roller::get_name(int number) const
{
    std::ostringstream stream;
    stream << file_writer_->get_file_name() << '.' << number;
    return stream.str();
}

void numbered_file_roller::roll()
{
    try
    {
        file::remove(get_name(max_index_));
        for (int i = max_index_; i > min_index_; i--)
        {
            std::string name = get_name(i - 1);
            if (file::exists(name))
                std::rename(name.c_str(), get_name(i).c_str());
        }
        std::rename(file_writer_->get_file_name().c_str(),
                    get_name(min_index_).c_str());
    }
    catch (std::exception& e)
    {
        std::throw_with_nested(exception("Could not roll the file " + file_writer_->get_file_name()));
    }
}

}
