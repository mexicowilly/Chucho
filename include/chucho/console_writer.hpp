#if !defined(CHUCHO_CONSOLE_WRITER__)
#define CHUCHO_CONSOLE_WRITER__

#include <chucho/writer.hpp>
#include <iosfwd>

namespace chucho
{

class CHUCHO_EXPORT console_writer : public writer
{
protected:
    console_writer(std::ostream& stream);

    virtual void write_impl(const event& evt) override;

    std::ostream& stream_;
};

class CHUCHO_EXPORT cout_writer : public console_writer
{
public:
    cout_writer();
};

class CHUCHO_EXPORT cerr_writer : public console_writer
{
public:
    cerr_writer();
};

}

#endif
