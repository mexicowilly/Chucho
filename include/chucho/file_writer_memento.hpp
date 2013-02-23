#if !defined(CHUCHO_FILE_WRITER_MEMENTO_HPP__)
#define CHUCHO_FILE_WRITER_MEMENTO_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/writer_memento.hpp>
#include <chucho/file_writer.hpp>
#include <chucho/optional.hpp>

namespace chucho
{

class file_writer_memento : public writer_memento
{
public:
    file_writer_memento(const configurator& cfg);

    const std::string& get_file_name() const;
    const optional<bool>& get_flush() const;
    const optional<file_writer::on_start>& get_on_start() const;

private:
    void set_on_start(const std::string& value);

    std::string file_name_;
    optional<file_writer::on_start> start_;
    optional<bool> flush_;
};

inline const std::string& file_writer_memento::get_file_name() const
{
    return file_name_;
}

inline const optional<bool>& file_writer_memento::get_flush() const
{
    return flush_;
}

inline const optional<file_writer::on_start>& file_writer_memento::get_on_start() const
{
    return start_;
}

}

#endif
