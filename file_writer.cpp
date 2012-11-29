#include <chucho/file_writer.hpp>

namespace chucho
{

file_exception::file_exception(const std::string& msg)
    : exception(msg)
{
}

file_writer::file_writer(on_start start)
    : start_(start)
{
}

file_writer::file_writer(const std::string& file_name, on_start start)
    : start_(start)
{
    open(file_name);
}

void file_writer::open(const std::string& file_name)
{
    file_name_ = file_name;
    file_.open(file_name, std::ios::out | ((start_ == on_start::APPEND) ? std::ios::app : std::ios::trunc));
    if (!file_.is_open())
        throw file_exception("Could not open " + file_name + " for writing");
    file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
}

void file_writer::write_impl(const event& evt)
{
    try
    {
        file_ << formatter_->format(evt);
    }
    catch (std::ios::failure&)
    {
        std::throw_with_nested(file_exception("Could not write to " + file_name_));
    }
}

}
