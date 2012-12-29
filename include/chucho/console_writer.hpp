#if !defined(CHUCHO_CONSOLE_WRITER__)
#define CHUCHO_CONSOLE_WRITER__

#include <chucho/writer.hpp>
#include <iosfwd>

namespace chucho
{

class CHUCHO_EXPORT console_writer : public writer
{
protected:
    console_writer(std::shared_ptr<formatter> fmt,
                   std::ostream& stream);

    virtual void write_impl(const event& evt) override;

    std::ostream& stream_;
};

}

#endif
