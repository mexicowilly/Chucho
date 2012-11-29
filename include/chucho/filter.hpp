#if !defined(CHUCHO_FILTER_HPP__)
#define CHUCHO_FILTER_HPP__

#include <chucho/event.hpp>

namespace chucho
{

class CHUCHO_EXPORT filter
{
public:
    enum class result
    {
        DENY,
        NEUTRAL,
        ACCEPT
    };

    virtual ~filter();

    virtual result evaluate(const event& evt) = 0;
};

}

#endif
