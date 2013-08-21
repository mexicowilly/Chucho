#include "winsock_startup.hpp"
#include <chucho/socket_exception.hpp>
#include <chucho/garbage_cleaner.hpp>
#include <winsock2.h>

namespace
{

class worker
{
public:
    worker()
    {
        WSADATA data;
        int rc = WSAStartup(MAKEWORD(2, 0), &data);
        if (rc != 0)
            throw chucho::socket_exception("Could not start socket subsystem", rc);
    }

    ~worker()
    {
        WSACleanup();
    }
};

}

namespace chucho
{

namespace winsock
{

std::once_flag once;

void startup()
{
    static worker* wrk = new worker();
    chucho::garbage_cleaner::get().add([&] () { delete wrk; });
}

}

}
