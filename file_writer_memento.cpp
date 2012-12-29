#include <chucho/file_writer_memento.hpp>

namespace chucho
{

file_writer_memento::file_writer_memento(const configurator& cfg)
    : writer_memento(cfg),
      start_(file_writer::on_start::APPEND),
      flush_(true)
{
    set_status_origin("file_writer_memento");
    set_handler("file_name", [this] (const std::string& name) { file_name_ = name; });
    set_handler("on_start", std::bind(&file_writer_memento::set_on_start, this, std::placeholders::_1));
    set_handler("flush", [this] (const std::string& val) { flush_ = boolean_value(val); });
}

void file_writer_memento::set_on_start(const std::string& value)
{
    std::string low = to_lower(value);
    if (low == "append")
        start_ = file_writer::on_start::APPEND;
    else if (low == "truncate")
        start_ = file_writer::on_start::TRUNCATE;
    else
        report_error("on_start has an invalid value of " + value);
}

}
