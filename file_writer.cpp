#include <chucho/file_writer.hpp>
#include <chucho/exception.hpp>
#include <chucho/file.hpp>

namespace chucho
{

file_writer::file_writer(std::shared_ptr<formatter> fmt,
                         on_start start,
                         bool flush)
    : writer(fmt),
      start_(start),
      flush_(flush)
{
    set_status_origin("file_writer");
}

file_writer::file_writer(std::shared_ptr<formatter> fmt,
                         const std::string& file_name,
                         on_start start,
                         bool flush)
    : writer(fmt),
      initial_file_name_(file_name),
      start_(start),
      flush_(flush)
{
    set_status_origin("file_writer");
    open(file_name);
}

void file_writer::open(const std::string& file_name)
{
    file_name_ = file_name;
    try
    {
        file::create_directories(file::directory_name(file_name));
        file_.open(file_name, std::ios::out | ((start_ == on_start::APPEND) ? std::ios::app : std::ios::trunc));
        if (file_.is_open())
            file_.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
        else
            report_error("Could not open " + file_name + " for writing");
    }
    catch (std::exception& e)
    {
        report_error("Error creating parent directories of " + file_name + ": " + e.what());
    }
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
