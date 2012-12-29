#if !defined(CHUCHO_FILE_WRITER_MEMENTO_HPP__)
#define CHUCHO_FILE_WRITER_MEMENTO_HPP__

#include <chucho/writer_memento.hpp>
#include <chucho/file_writer.hpp>

namespace chucho
{

class CHUCHO_EXPORT file_writer_memento : public writer_memento
{
public:
    file_writer_memento(const configurator& cfg);

    const std::string& get_file_name() const;
    bool get_flush() const;
    file_writer::on_start get_on_start() const;

private:
    void set_on_start(const std::string& value);

    std::string file_name_;
    file_writer::on_start start_;
    bool flush_;
};

inline const std::string& file_writer_memento::get_file_name() const
{
    return file_name_;
}

inline bool file_writer_memento::get_flush() const
{
    return flush_;
}

inline file_writer::on_start file_writer_memento::get_on_start() const
{
    return start_;
}

}

#endif
