#if !defined(CHUCHO_FILE_WRITER__)
#define CHUCHO_FILE_WRITER__

#include <chucho/writer.hpp>
#include <fstream>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT file_writer : public writer
{
public:
    enum class on_start
    {
        APPEND,
        TRUNCATE
    };

    file_writer(std::shared_ptr<formatter> fmt,
                const std::string& file_name,
                on_start start = on_start::APPEND,
                bool flush = true);

    const std::string& get_file_name() const;
    const std::string& get_initial_file_name() const;

protected:
    file_writer(std::shared_ptr<formatter> fmt,
                on_start start,
                bool flush);

    void close();
    void open(const std::string& file_name);
    virtual void write_impl(const event& evt) override;

private:
    std::string initial_file_name_;
    std::string file_name_;
    std::ofstream file_;
    on_start start_;
    bool flush_;
};

inline void file_writer::close()
{
    file_.close();
}

inline const std::string& file_writer::get_file_name() const
{
    return file_name_;
}

inline const std::string& file_writer::get_initial_file_name() const
{
    return initial_file_name_;
}

}

#endif
