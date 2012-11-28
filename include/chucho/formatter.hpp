#if !defined(CHUCHO_FORMATTER_HPP__)
#define CHUCHO_FORMATTER_HPP__

#include <chucho/event.hpp>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT formatter
{
public:
    virtual ~formatter();

    virtual std::string format(const event& evt) = 0;
};

}

#endif
