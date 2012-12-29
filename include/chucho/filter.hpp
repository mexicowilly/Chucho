#if !defined(CHUCHO_FILTER_HPP__)
#define CHUCHO_FILTER_HPP__

#include <chucho/event.hpp>
#include <chucho/status_reporter.hpp>
#include <chucho/configurable.hpp>

namespace chucho
{

class CHUCHO_EXPORT filter : public status_reporter,
                             public configurable
{
public:
    enum class result
    {
        DENY,
        NEUTRAL,
        ACCEPT
    };

    virtual result evaluate(const event& evt) = 0;
};

}

#endif
