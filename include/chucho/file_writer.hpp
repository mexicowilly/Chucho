#if !defined(CHUCHO_FILE_WRITER__)
#define CHUCHO_FILE_WRITER__

#include <chucho/writer.hpp>
#include <chucho/exception.hpp>
#include <fstream>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT file_exception : public exception
{
public:
    file_exception(const std::string& msg);
};

class CHUCHO_EXPORT file_writer : public writer
{
public:
    enum class on_start
    {
        APPEND,
        TRUNCATE
    };

    file_writer(const std::string& file_name, on_start start);

    const std::string& get_file_name() const;

protected:
    file_writer(on_start start);

    void close();
    void open(const std::string& file_name);
    virtual void write_impl(const event& evt) override;

private:
    std::string file_name_;
    std::ofstream file_;
    on_start start_;
};

inline void file_writer::close()
{
    file_.close();
}

inline const std::string& file_writer::get_file_name() const
{
    return file_name_;
}

}

#endif
