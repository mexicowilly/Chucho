#if !defined(CHUCHO_STATUS_OBSERVER_HPP__)
#define CHUCHO_STATUS_OBSERVER_HPP__

#include <chucho/status.hpp>

namespace chucho
{

class CHUCHO_EXPORT status_observer
{
public:
    virtual ~status_observer();

    virtual void status_reported(const status& st) = 0;
};

}

#endif
