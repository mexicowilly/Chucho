#include <chucho/file_writer.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

file_writer::file_writer(on_start start,
                         bool flush)
    : start_(start),
      flush_(flush)
{
}

file_writer::file_writer(const std::string& file_name,
                         on_start start,
                         bool flush)
    : initial_file_name_(file_name),
      start_(start),
      flush_(flush)
{
    open(file_name);
}

void file_writer::open(const std::string& file_name)
{
    file_name_ = file_name;
    file_.open(file_name, std::ios::out | ((start_ == on_start::APPEND) ? std::ios::app : std::ios::trunc));
    if (file_.is_open())
        file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    else
        report_error("Could not open " + file_name + " for writing");
}

void file_writer::write_impl(const event& evt)
{
    try
    {
        if (file_.is_open())
        {
            file_ << formatter_->format(evt);
            if (flush_)
                file_.flush();
        }
    }
    catch (std::ios::failure&)
    {
        std::throw_with_nested(file_exception("Could not write to " + file_name_));
    }
}

}
